#include "xui_native_device.h"
#include "xui_timermgr.h"
#include "xui_native_window.h"
#include "xui_global.h"
#include "glfw3.h"
#ifndef GLFW_EXPOSE_NATIVE_NSGL
#define GLFW_EXPOSE_NATIVE_NSGL
#endif
#ifndef GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include "glfw3native.h"

#import <Cocoa/Cocoa.h>
#import <CoreServices/CoreServices.h>

void fsevents_callback(
    ConstFSEventStreamRef streamRef,
    void *userData,
    size_t numEvents,
    void *eventPaths,
    const FSEventStreamEventFlags eventFlags[],
    const FSEventStreamEventId eventIds[]);

@interface FileMonitor : NSObject

+ (instancetype)sharedInstance;
- (void)startWatch:(NSString*)directory;
- (void)stopWatch;

@property(nonatomic) NSInteger syncEventID;
@property(nonatomic, assign) FSEventStreamRef syncEventStream;

@end

@implementation FileMonitor

+ (instancetype)sharedInstance
{
    static FileMonitor *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^
    {
        sharedInstance = [[FileMonitor alloc] init];
    });
    return sharedInstance;
}

- (void)startWatch:(NSString*)directory
{
    if(self.syncEventStream)
    {
        FSEventStreamStop(self.syncEventStream);
        FSEventStreamInvalidate(self.syncEventStream);
        FSEventStreamRelease(self.syncEventStream);
        self.syncEventStream = NULL;
    }
    NSArray* paths = @[directory];// 这里填入需要监控的文件夹
    FSEventStreamContext context;
    context.info = (__bridge void * _Nullable)(self);
    context.version = 0;
    context.retain = NULL;
    context.release = NULL;
    context.copyDescription = NULL;
    self.syncEventStream = FSEventStreamCreate(
        NULL,
        &fsevents_callback, &context,
        (__bridge CFArrayRef _Nonnull)(paths),
        kFSEventStreamEventIdSinceNow,
        1,
        kFSEventStreamCreateFlagUseCFTypes | kFSEventStreamCreateFlagFileEvents);
    
    FSEventStreamScheduleWithRunLoop(self.syncEventStream, CFRunLoopGetMain(), kCFRunLoopDefaultMode);
    FSEventStreamStart(self.syncEventStream);
}

- (void)stopWatch
{
    if(self.syncEventStream)
    {
        FSEventStreamStop(self.syncEventStream);
        FSEventStreamInvalidate(self.syncEventStream);
        FSEventStreamRelease(self.syncEventStream);
        self.syncEventStream = NULL;
    }
}

-(void)updateEventID
{
    self.syncEventID = FSEventStreamGetLatestEventId(self.syncEventStream);
}

-(void)setSyncEventID:(NSInteger)syncEventID
{
    [[NSUserDefaults standardUserDefaults] setInteger:syncEventID forKey:@"SyncEventID"];
}

-(NSInteger)syncEventID
{
    NSInteger syncEventID = [[NSUserDefaults standardUserDefaults] integerForKey:@"SyncEventID"];
    //if(syncEventID == 0)
    {
        syncEventID = kFSEventStreamEventIdSinceNow;
    }
    return syncEventID;
}
@end

void fsevents_callback(
    ConstFSEventStreamRef streamRef,
    void *userData,
    size_t numEvents,
    void *eventPaths,
    const FSEventStreamEventFlags eventFlags[],
    const FSEventStreamEventId eventIds[])
{
    FileMonitor *self = (__bridge FileMonitor *)userData;
    NSArray *pathArr = (__bridge NSArray*)eventPaths;
    
    for (int i = 0; i < numEvents; ++i)
    {
        FSEventStreamEventFlags flag = eventFlags[i];
        
        u32 eventid = 0;
        if(kFSEventStreamEventFlagItemCreated & flag)
        {
        }
        if(kFSEventStreamEventFlagItemRenamed & flag)
        {
            //FSEventStreamEventId currentEventID = eventIds[i];
            //NSString* currentPath = pathArr[i];
            //if (currentEventID == lastRenameEventID + 1)
            //{
            //}
            //else
            //{
            //    if ([[NSFileManager defaultManager] fileExistsAtPath:currentPath])
            //    {
            //    }
            //    else
            //    {
            //    }
            //}
        }
        if(kFSEventStreamEventFlagItemRemoved & flag)
        {
        }
        if(kFSEventStreamEventFlagItemModified & flag)
        {
        }
        
        std::string utf8Path = [pathArr[i] UTF8String];
        std::wstring unicodePath = xui_global::utf8_to_unicode(utf8Path);
        xui_native_device::add_fwatch(eventid, unicodePath, unicodePath);
    }
    
    [self updateEventID];
}

xui_vector<s32> xui_native_device::get_screen_size( void )
{
    NSRect screenRect = [[NSScreen mainScreen] frame];
    return xui_vector<s32>(screenRect.size.width, screenRect.size.height);
}

void xui_native_device::capture_screenshot( void )
{
    NSRect screenRect = [[NSScreen mainScreen] frame];

    // Create a CGImage with the screen contents
    CGImageRef cgImage = CGWindowListCreateImage(screenRect, kCGWindowListOptionOnScreenOnly, kCGNullWindowID, kCGWindowImageDefault);
    
    // Convert the CGImage into a NSBitmapImageRep
    NSBitmapImageRep* rep = [[NSBitmapImageRep alloc] initWithCGImage:cgImage];
    
    // Release the CGImage
    CGImageRelease(cgImage);
    
    // Create a NSImage
    NSImage* image = [[NSImage alloc] init];
    
    // Add the NSBitmapImageRep
    [image addRepresentation:rep];
    
    // Create a context to hold the image data
    CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    CGContextRef ctx = CGBitmapContextCreate(
        NULL,
        screenRect.size.width,
        screenRect.size.height,
        8,
        0,
        colorSpace,
        kCGImageAlphaPremultipliedLast);

    // Wrap graphics context
    NSGraphicsContext* gctx = [NSGraphicsContext graphicsContextWithCGContext:ctx flipped:NO];

    // Make our bitmap context current and render the NSImage into it
    [NSGraphicsContext setCurrentContext:gctx];
    [image drawInRect:screenRect];

    // Calculate the histogram
    int screen_w = (s32)(CGBitmapContextGetWidth(ctx));
    int screen_h = (s32)(CGBitmapContextGetHeight(ctx));
    s_screen_buf = new u08[screen_w * screen_h * 3];

    uint32_t* pixel = (uint32_t*)CGBitmapContextGetData(ctx);
    u08* writeBuffer = s_screen_buf;
    for (int y = 0; y < screen_h; ++y)
    {
        for (int x = 0; x < screen_w; ++x)
        {
            u32 rgba = *pixel;

            // Extract colour components
            u08 red   = (rgba & 0x000000ff) >> 0;
            u08 green = (rgba & 0x0000ff00) >> 8;
            u08 blue  = (rgba & 0x00ff0000) >> 16;
            
            *(writeBuffer++) = blue;
            *(writeBuffer++) = green;
            *(writeBuffer++) = red;

            // Next pixel!
            pixel++;
        }
    }

    // Clean up
    [NSGraphicsContext setCurrentContext:nil];
    CGContextRelease(ctx);
    CGColorSpaceRelease(colorSpace);
}

void xui_native_device::set_fwatchstart( const std::wstring& path )
{
    std::string utf8Path = xui_global::unicode_to_utf8(path);
    NSString* nsPath = [NSString stringWithUTF8String:utf8Path.c_str()];
    [[FileMonitor sharedInstance] startWatch:nsPath];
}

void xui_native_device::set_fwatchclose( void )
{
    [[FileMonitor sharedInstance] stopWatch];
}

void xui_native_device::set_openpath_dialog( xui_method<const std::wstring>* callback )
{
    id mainWindow = glfwGetCocoaWindow(xui_native_window::get_global_window()->get_glfw_window());
    
    NSOpenPanel* openPanel = [NSOpenPanel openPanel];
    openPanel.allowsMultipleSelection = false;
    openPanel.canCreateDirectories = true;
    openPanel.canChooseDirectories = true;
    openPanel.canChooseFiles = false;
    openPanel.allowsMultipleSelection = false;
    openPanel.message = [NSString stringWithUTF8String:"请选择工作目录"];
    openPanel.prompt = [NSString stringWithUTF8String:"打开"];
    openPanel.directoryURL = [NSURL fileURLWithPath:NSHomeDirectory() isDirectory:YES];
    
    [openPanel beginSheetModalForWindow:mainWindow completionHandler:^(NSInteger result)
    {
        if (result == NSModalResponseOK)
        {
            NSArray* fileURLs = [openPanel URLs];
            NSURL* fileURL = [fileURLs objectAtIndex:0];
            NSString* directory = [fileURL path];
            std::string workdir = [directory UTF8String];
            (*callback)(NULL, xui_global::utf8_to_unicode(workdir));
        }
    }];
}

std::string xui_native_device::get_temppath( void )
{
    NSString* homePath = NSHomeDirectory();
    return std::string([homePath UTF8String]) + "/Documents/";
}

const std::wstring& xui_native_device::get_workpath( void )
{
    return s_work_path;
}

void xui_native_device::set_workpath( const std::wstring& path )
{
    std::string tmp = xui_global::unicode_to_utf8(pathstyle_replace(path, L'\\', L'/'));
    
    NSFileManager* fileManager = [NSFileManager defaultManager];
    NSString* directoryPath = [NSString stringWithUTF8String:tmp.c_str()];
    [fileManager changeCurrentDirectoryPath:directoryPath];
    
    s_work_path = pathstyle_replace(path, L'\\', L'/') + L"/";
}

static NSString* get_full_path( const std::wstring& path )
{
    std::string utf8Path = xui_global::unicode_to_utf8(path);
    NSString* nsPath = [NSString stringWithUTF8String:utf8Path.c_str()];
    //relative path
    if (path.empty() || path.find(L"/") != 0)
    {
        NSString* workingPath = [[NSFileManager defaultManager] currentDirectoryPath];
        nsPath = [workingPath stringByAppendingPathComponent:nsPath];
    }
    return nsPath;
}

std::vector<std::wstring> get_path_items( const std::wstring& path, BOOL isDir, const std::wstring& filter = L"" )
{
    std::vector<std::string> filevec;
    
    NSFileManager* fileManager = [NSFileManager defaultManager];
    
    NSString* nsPath = get_full_path(path);
    
    std::string utf8Filter = xui_global::unicode_to_utf8(filter);
    NSString* nsFilter = [NSString stringWithUTF8String:utf8Filter.c_str()];
    
    NSURL* directoryURL = [NSURL fileURLWithPath:nsPath isDirectory:YES];
    //BOOL isAccessing = [directoryURL startAccessingSecurityScopedResource];
    NSArray* keys = [NSArray array];
        
    NSArray* fileList = [fileManager contentsOfDirectoryAtURL:directoryURL includingPropertiesForKeys:keys options:0 error:nil];
    if (fileList)
    {
        for (NSURL* s in fileList)
        {
            if ([s hasDirectoryPath] == isDir)
            {
                NSString* lastPathComp = [s lastPathComponent];
                if ([nsFilter length] == 0 || [lastPathComp isCaseInsensitiveLike:nsFilter])
                {
                    const char* cLastPathComp = [lastPathComp UTF8String];
                    filevec.push_back(std::string(cLastPathComp));
                }
            }
        }
    }
    
    struct case_insensitive_less : public std::binary_function< char,char,bool >
    {
        bool operator () (char x, char y) const
        {
            return toupper( static_cast< unsigned char >(x)) <
                   toupper( static_cast< unsigned char >(y));
        }
    };
    
    std::sort(filevec.begin(), filevec.end(), [](const std::string &a, const std::string &b)
    {
        return std::lexicographical_compare(a.begin(),a.end(), b.begin(),b.end(), case_insensitive_less());
    });
    
    std::vector<std::wstring> result;
    for (const std::string& file : filevec)
    {
        result.push_back(xui_global::utf8_to_unicode(file));
    }
    
    return result;
}

void xui_native_device::set_showfind( const std::wstring& path )
{
    NSString* nsPath  = get_full_path(path);
    NSURL* nsPathURL  = [NSURL fileURLWithPath:nsPath];
    NSArray* fileURLs = [NSArray arrayWithObjects:nsPathURL, nil];
    [[NSWorkspace sharedWorkspace] activateFileViewerSelectingURLs:fileURLs];
}

std::vector<std::wstring> xui_native_device::get_path( const std::wstring& path )
{
    return get_path_items(path, YES);
}

std::vector<std::wstring> xui_native_device::get_file( const std::wstring& path, const std::wstring& filter )
{
    return get_path_items(path, NO, filter);
}

bool xui_native_device::has_path( const std::wstring& path )
{
    NSString* nsPath = get_full_path(path);
    return [[NSFileManager defaultManager] fileExistsAtPath:nsPath];
}

bool xui_native_device::add_path( const std::wstring& path )
{
    NSString* nsPath = get_full_path(path);

    NSError* error;
    if ([[NSFileManager defaultManager] createDirectoryAtPath:nsPath withIntermediateDirectories:NO attributes:nil error:&error])
	{
        return true;
    }
    else
	{
        NSLog(@"Create error: %@", error);
        return false;
    }
}

bool xui_native_device::del_file( const std::wstring& path )
{
    NSString* nsPath = get_full_path(path);

    NSError *error;
    if ([[NSFileManager defaultManager] removeItemAtPath:nsPath error:&error])
	{
        return true;
    }
    else
	{
        NSLog(@"Remove error: %@", error);
        return false;
    }
}

bool xui_native_device::mov_file( const std::wstring& src, const std::wstring& dst )
{
    NSString* nsSrc = get_full_path(src);
    NSString* nsDst = get_full_path(dst);

    NSError* error;
    if ([[NSFileManager defaultManager] moveItemAtPath:nsSrc toPath:nsDst error:&error])
	{
        return true;
    }
    else
	{
        NSLog(@"Move error: %@", error);
        return false;
    }
}

bool xui_native_device::cpy_file( const std::wstring& src, const std::wstring& dst )
{
    NSString* nsSrc = get_full_path(src);
    NSString* nsDst = get_full_path(dst);

    NSError* error;
    if ([[NSFileManager defaultManager] copyItemAtPath:nsSrc toPath:nsDst error:&error])
	{
        return true;
    }
    else
	{
        NSLog(@"Copy error: %@", error);
        return false;
    }
}

bool xui_native_device::rna_file( const std::wstring& src, const std::wstring& dst )
{
    NSString* nsSrc = get_full_path(src);
    NSString* nsDst = get_full_path(dst);

    NSError* error;
    if ([[NSFileManager defaultManager] moveItemAtPath:nsSrc toPath:nsDst error:&error])
	{
        return true;
    }
    else
	{
        NSLog(@"Rename error: %@", error);
        return false;
    }
}

void xui_native_device::sleep( f32 seconds )
{
    [NSThread sleepForTimeInterval:seconds];
}

void xui_native_device::release_capture( void )
{}
