#ifndef __cocos_propmaterial_h__
#define __cocos_propmaterial_h__

#include "cocos_propfile.h"

namespace cocos2d
{
	class Properties;
	class Texture2D;
}

class cocos_propvertshader;
class cocos_propfragshader;
class cocos_propmaterial : public cocos_propfile
{
public:
	/*
	//uniform
	*/
	struct uniform
	{
		std::string					name;
		std::vector<f32>			data;
	};

	/*
	//sampler
	*/
	struct sampler
	{
		std::string					name;
		cocos2d::Texture2D*			texture;
		bool						mipmap;
		s32							wraps;
		s32							wrapt;
		s32							minfilter;
		s32							magfilter;

		sampler( void )
		: texture(NULL)
		, mipmap(false)
		, wraps(GL_REPEAT)
		, wrapt(GL_REPEAT)
		, minfilter(GL_NEAREST)
		, magfilter(GL_NEAREST)
		{}
	};

	/*
	//shader
	*/
	struct glprogram
	{
		cocos_propfile*				vertshader;
		std::string					vertshaderfile;
		std::vector<uniform>		vertuniforms;
		cocos_propfile*				fragshader;
		std::string					fragshaderfile;
		std::vector<uniform>		fraguniforms;
		std::vector<sampler>		fragsamplers;
	};

	/*
	//pass
	*/
	struct pass
	{
		glprogram					program;
		bool						blend;
		s32							blendsrc;
		s32							blenddst;
		bool						depthtest;
		bool						depthwrite;
		s32							depthfunc;
		bool						cullface;
		s32							cullfaceside;
		s32							frontface;
	};

	/*
	//constructor
	*/
	cocos_propmaterial( const std::wstring& full );

	/*
	//destructor
	*/
	virtual ~cocos_propmaterial( void );

	/*
	//override
	*/
	cocos_propvertshader*			get_vertshader		( void );
	void							set_vertshader		( cocos_propvertshader* shader );
	cocos_propfragshader*			get_fragshader		( void );
	void							set_fragshader		( cocos_propfragshader* shader );
	void							ntf_vertshadermodify( void );
	void							ntf_fragshadermodify( void );
	virtual std::string				get_dragtype		( void );
	virtual void*					get_dragdata		( void );
	virtual bool					was_modify			( void );
	virtual void					load				( void );

protected:
	/*
	//event
	*/
	void							on_propchanged		( xui_component* sender, xui_method_propdata&	args );
	void							on_namechanged		( xui_component* sender, xui_method_args&		args );

	/*
	//override
	*/
	virtual void					save_as				( const std::wstring& fullname, bool modify );

	/*
	//method
	*/
	void							parse_material		( cocos2d::Properties* root );
	void							parse_technique		( cocos2d::Properties* root );
	void							parse_pass			( cocos2d::Properties* root );
	void							parse_state			( cocos2d::Properties* root );
	void							parse_program		( cocos2d::Properties* root );
	void							parse_sampler		( cocos2d::Properties* root, cocos_propmaterial::sampler* sampler_value );
	void							parse_uniform		( cocos2d::Properties* root, cocos_propmaterial::uniform* uniform_value );
	std::string						parse_optional		( cocos2d::Properties* root, const char* key, const std::string& default_value );
	s32								parse_textmap		( const std::string& value, const std::map<s32, std::wstring>& textmap );
	void							parse_shader		( const std::string& fullname, std::vector<cocos_propmaterial::uniform>* uniform_vec, std::vector<cocos_propmaterial::sampler>* sampler_vec );

	/*
	//static
	*/
	static void*					get_vertshader		( void* userptr );
	static void						set_vertshader		( void* userptr, void* value );
	static void*					get_fragshader		( void* userptr );
	static void						set_fragshader		( void* userptr, void* value );

	/*
	//member
	*/
	std::wstring					m_name;
	std::wstring					m_tech;
	pass							m_pass;
	bool							m_modify;
	xui_propkind*					m_namekind;
	xui_propkind*					m_techkind;
	xui_propkind*					m_vertkind;
	xui_propkind*					m_fragkind;
};

#endif//__onity_propmaterial_h__