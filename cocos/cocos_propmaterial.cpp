#include "renderer/CCMaterial.h"
#include "base/CCProperties.h"
#include "base/CCDirector.h"
#include "renderer/CCTextureCache.h"
#include "renderer/CCTexture2D.h"
#include "xui_global.h"
#include "xui_propctrl.h"
#include "xui_kindctrl.h"
#include "xui_propkind.h"
#include "xui_propctrl_expand.h"
#include "xui_textbox.h"
#include "cocos_filedata.h"
#include "cocos_propdata_shader.h"
#include "cocos_propctrl_uniform.h"
#include "cocos_propctrl_sampler.h"
#include "cocos_propvertshader.h"
#include "cocos_propfragshader.h"
#include "cocos_resource.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_inspector.h"
#include "cocos_propmaterial.h"

const std::map<s32, std::wstring> text_blend = {
	{ cocos2d::RenderState::BLEND_ZERO,						L"ZERO"						},
	{ cocos2d::RenderState::BLEND_ONE,						L"ONE"						},
	{ cocos2d::RenderState::BLEND_SRC_COLOR,				L"SRC_COLOR"				},
	{ cocos2d::RenderState::BLEND_ONE_MINUS_SRC_COLOR,		L"ONE_MINUS_SRC_COLOR"		},
	{ cocos2d::RenderState::BLEND_DST_COLOR,				L"DST_COLOR"				},
	{ cocos2d::RenderState::BLEND_ONE_MINUS_DST_COLOR,		L"ONE_MINUS_DST_COLOR"		},
	{ cocos2d::RenderState::BLEND_SRC_ALPHA,				L"SRC_ALPHA"				},
	{ cocos2d::RenderState::BLEND_ONE_MINUS_SRC_ALPHA,		L"ONE_MINUS_SRC_ALPHA"		},
	{ cocos2d::RenderState::BLEND_DST_ALPHA,				L"DST_ALPHA"				},
	{ cocos2d::RenderState::BLEND_ONE_MINUS_DST_ALPHA,		L"ONE_MINUS_DST_ALPHA"		},
	{ cocos2d::RenderState::BLEND_CONSTANT_ALPHA,			L"CONSTANT_ALPHA"			},
	{ cocos2d::RenderState::BLEND_ONE_MINUS_CONSTANT_ALPHA,	L"ONE_MINUS_CONSTANT_ALPHA"	},
	{ cocos2d::RenderState::BLEND_SRC_ALPHA_SATURATE,		L"SRC_ALPHA_SATURATE"		}
};
const std::map<s32, std::wstring> text_depth = {
	{ cocos2d::RenderState::DEPTH_NEVER,					L"NEVER"					},	
	{ cocos2d::RenderState::DEPTH_LESS,						L"LESS"						},	
	{ cocos2d::RenderState::DEPTH_EQUAL,					L"EQUAL"					},	
	{ cocos2d::RenderState::DEPTH_LEQUAL,					L"LEQUAL"					},
	{ cocos2d::RenderState::DEPTH_GREATER,					L"GREATER"					},
	{ cocos2d::RenderState::DEPTH_NOTEQUAL,					L"NOTEQUAL"					},
	{ cocos2d::RenderState::DEPTH_GEQUAL,					L"GEQUAL"					},
	{ cocos2d::RenderState::DEPTH_ALWAYS,					L"ALWAYS"					}
};
const std::map<s32, std::wstring> text_cullfaceside = {
	{ cocos2d::RenderState::CULL_FACE_SIDE_BACK,			L"BACK"						},
	{ cocos2d::RenderState::CULL_FACE_SIDE_FRONT,			L"FRONT"					},
	{ cocos2d::RenderState::CULL_FACE_SIDE_FRONT_AND_BACK,	L"FRONT_AND_BACK"			}
};
const std::map<s32, std::wstring> text_frontface = {
	{ cocos2d::RenderState::FRONT_FACE_CCW,					L"CCW"						},
	{ cocos2d::RenderState::FRONT_FACE_CW,					L"CW"						}
};

/*
//constructor
*/
xui_create_explain(cocos_propmaterial)( const std::wstring& full )
: cocos_propfile(cocos_resource::icon_animator, full)
, m_modify(false)
, m_namekind(NULL)
, m_techkind(NULL)
, m_vertkind(NULL)
, m_fragkind(NULL)
{
	load();

	m_namekind = new xui_propkind(this, m_name, "Material", xui_kindctrl::create, cocos_resource::icon_ai, true);
	xui_method_ptrcall(m_namekind, xm_namechanged) += new xui_method_member<xui_method_args,		cocos_propmaterial>(this, &cocos_propmaterial::on_namechanged);
	xui_method_ptrcall(m_namekind, xm_propchanged) += new xui_method_member<xui_method_propdata,	cocos_propmaterial>(this, &cocos_propmaterial::on_propchanged);
	m_namekind->add_propdata(new xui_propdata_string(
		m_namekind,
		L"Technique",
		xui_propctrl_string::create,
		NULL,
		NULL,
		&m_tech));

	m_techkind = new xui_propkind(this, L"Render State", "RenderState", xui_kindctrl::create, cocos_resource::icon_animator, true);
	xui_method_ptrcall(m_techkind, xm_propchanged) += new xui_method_member<xui_method_propdata,	cocos_propmaterial>(this, &cocos_propmaterial::on_propchanged);

	//blend
	xui_propdata_vec				subprop;
	std::map<s32, std::vector<u32>>	showmap;
	subprop.push_back(new xui_propdata_enum_impl<s32>(
		m_techkind,
		L"BlendSrc",
		xui_propctrl_enum::create,
		text_blend,
		&m_pass.blendsrc));
	subprop.push_back(new xui_propdata_enum_impl<s32>(
		m_techkind,
		L"BlendDst",
		xui_propctrl_enum::create,
		text_blend,
		&m_pass.blenddst));
	showmap[1].push_back(0);
	showmap[1].push_back(1);
	m_techkind->add_propdata(new xui_propdata_expand_bool(
		m_techkind,
		L"Blend",
		xui_propctrl_expand_bool::create,
		subprop,
		NULL,
		NULL,
		&m_pass.blend,
		true,
		showmap));

	subprop.clear();
	subprop.push_back(new xui_propdata_bool(
		m_techkind,
		L"DepthWrite",
		xui_propctrl_bool::create,
		NULL,
		NULL,
		&m_pass.depthwrite));
	subprop.push_back(new xui_propdata_enum_impl<s32>(
		m_techkind,
		L"DepthFunc",
		xui_propctrl_enum::create,
		text_depth,
		&m_pass.depthfunc));
	showmap.clear();
	showmap[1].push_back(0);
	showmap[1].push_back(1);
	m_techkind->add_propdata(new xui_propdata_expand_bool(
		m_techkind,
		L"DepthTest",
		xui_propctrl_expand_bool::create,
		subprop,
		NULL,
		NULL,
		&m_pass.depthtest,
		true,
		showmap));

	//cull
	subprop.clear();
	subprop.push_back(new xui_propdata_enum_impl<s32>(
		m_techkind,
		L"CullFaceSide",
		xui_propctrl_enum::create,
		text_cullfaceside,
		&m_pass.cullfaceside));
	subprop.push_back(new xui_propdata_enum_impl<s32>(
		m_techkind,
		L"FrontFace",
		xui_propctrl_enum::create,
		text_frontface,
		&m_pass.frontface));
	showmap.clear();
	showmap[1].push_back(0);
	showmap[1].push_back(1);
	m_techkind->add_propdata(new xui_propdata_expand_bool(
		m_techkind,
		L"CullFace",
		xui_propctrl_expand_bool::create,
		subprop,
		NULL,
		NULL,
		&m_pass.cullface,
		true,
		showmap));

	m_vertkind = new xui_propkind(this, L"Vertex", "MatVertShader", xui_kindctrl::create, cocos_resource::icon_input, true);
	xui_method_ptrcall(m_vertkind, xm_propchanged) += new xui_method_member<xui_method_propdata,	cocos_propmaterial>(this, &cocos_propmaterial::on_propchanged);
	m_vertkind->add_propdata(new cocos_propdata_vertshader(
		m_vertkind,
		L"Shader",
		get_vertshader,
		set_vertshader,
		this));
	m_vertkind->add_propdata(new cocos_propdata_uniform(
		m_vertkind,
		L"Uniforms",
		&m_pass.program.vertuniforms));

	m_fragkind = new xui_propkind(this, L"Fragment", "MatFragShader", xui_kindctrl::create, cocos_resource::icon_input, true);
	xui_method_ptrcall(m_fragkind, xm_propchanged) += new xui_method_member<xui_method_propdata, cocos_propmaterial>(this, &cocos_propmaterial::on_propchanged);
	m_fragkind->add_propdata(new cocos_propdata_fragshader(
		m_fragkind,
		L"Shader",
		get_fragshader,
		set_fragshader,
		this));
	m_fragkind->add_propdata(new cocos_propdata_uniform(
		m_fragkind,
		L"Uniforms",
		&m_pass.program.fraguniforms));
	m_fragkind->add_propdata(new cocos_propdata_sampler(
		m_fragkind,
		L"Samplers",
		&m_pass.program.fragsamplers));

	add_propkind(m_namekind);
	add_propkind(m_techkind);
	add_propkind(m_vertkind);
	add_propkind(m_fragkind);
}

/*
//destructor
*/
xui_delete_explain(cocos_propmaterial)( void )
{}

/*
//override
*/
xui_method_explain(cocos_propmaterial, get_vertshader,			cocos_propvertshader*	)( void )
{
	if (m_pass.program.vertshader == NULL && m_pass.program.vertshaderfile.length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_pass.program.vertshaderfile);
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length() - 1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_pass.program.vertshader = project->get_pathfile(path, file);
	}

	return dynamic_cast<cocos_propvertshader*>(m_pass.program.vertshader);
}
xui_method_explain(cocos_propmaterial, set_vertshader,			void					)( cocos_propvertshader* shader )
{
	m_pass.program.vertshader = NULL;
	xui_method_refcall(m_pass.program.vertshaderfile,	clear)();
	xui_method_refcall(m_pass.program.vertuniforms,		clear)();
	if (shader)
	{
		m_pass.program.vertshader = shader;
		m_pass.program.vertshaderfile = xui_global::unicode_to_ascii(shader->get_fullname());
	}

	ntf_vertshadermodify();
}
xui_method_explain(cocos_propmaterial, get_fragshader,			cocos_propfragshader*	)( void )
{
	if (m_pass.program.fragshader == NULL && m_pass.program.fragshaderfile.length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_pass.program.fragshaderfile);
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length() - 1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_pass.program.fragshader = project->get_pathfile(path, file);
	}

	return dynamic_cast<cocos_propfragshader*>(m_pass.program.fragshader);
}
xui_method_explain(cocos_propmaterial, set_fragshader,			void					)( cocos_propfragshader* shader )
{
	m_pass.program.fragshader = NULL;
	xui_method_refcall(m_pass.program.fragshaderfile,	clear)();
	xui_method_refcall(m_pass.program.fraguniforms,		clear)();
	xui_method_refcall(m_pass.program.fragsamplers,		clear)();
	if (shader)
	{
		m_pass.program.fragshader = shader;
		m_pass.program.fragshaderfile = xui_global::unicode_to_ascii(shader->get_fullname());
		parse_shader(m_pass.program.fragshaderfile, &m_pass.program.fraguniforms, &m_pass.program.fragsamplers);
	}

	ntf_fragshadermodify();
}
xui_method_explain(cocos_propmaterial, ntf_vertshadermodify,	void					)( void )
{
	if (m_pass.program.vertshaderfile.length() > 0)
	{
		std::vector<uniform> uniform_vec;
		parse_shader(m_pass.program.vertshaderfile, &uniform_vec, NULL);
		for (u32 i = 0; i < uniform_vec.size(); ++i)
		{
			std::vector<uniform>::iterator itor = m_pass.program.vertuniforms.begin();
			for (; itor != m_pass.program.vertuniforms.end(); ++itor)
			{
				if (uniform_vec[i].name == (*itor).name)
				{
					uniform_vec[i].data =  (*itor).data;
					break;
				}
			}
		}

		m_pass.program.vertuniforms = uniform_vec;
	}

	xui_propdata_vec vec = m_vertkind->get_propdata();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		cocos_propdata_uniform* datauniform = dynamic_cast<cocos_propdata_uniform*>(vec[i]);
		if (datauniform)
			datauniform->reset();
	}
}
xui_method_explain(cocos_propmaterial, ntf_fragshadermodify,	void					)( void )
{
	if (m_pass.program.fragshaderfile.length() > 0)
	{
		std::vector<uniform> uniform_vec;
		std::vector<sampler> sampler_vec;
		parse_shader(m_pass.program.fragshaderfile, &uniform_vec, &sampler_vec);
		for (u32 i = 0; i < uniform_vec.size(); ++i)
		{
			std::vector<uniform>::iterator itor = m_pass.program.fraguniforms.begin();
			for (; itor != m_pass.program.fraguniforms.end(); ++itor)
			{
				if (uniform_vec[i].name == (*itor).name)
				{
					uniform_vec[i] = (*itor);
					break;
				}
			}
		}
		for (u32 i = 0; i < sampler_vec.size(); ++i)
		{
			std::vector<sampler>::iterator itor = m_pass.program.fragsamplers.begin();
			for (; itor != m_pass.program.fragsamplers.end(); ++itor)
			{
				if (sampler_vec[i].name == (*itor).name)
				{
					sampler_vec[i] = (*itor);
					break;
				}
			}
		}

		m_pass.program.fraguniforms = uniform_vec;
		m_pass.program.fragsamplers = sampler_vec;
	}

	xui_propdata_vec vec = m_fragkind->get_propdata();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		cocos_propdata_uniform* datauniform = dynamic_cast<cocos_propdata_uniform*>(vec[i]);
		if (datauniform)
			datauniform->reset();
		cocos_propdata_sampler* datasampler = dynamic_cast<cocos_propdata_sampler*>(vec[i]);
		if (datasampler)
			datasampler->reset();
	}
}
xui_method_explain(cocos_propmaterial, get_dragtype,			std::string				)( void )
{
	return "Material";
}
xui_method_explain(cocos_propmaterial, get_dragdata,			void*					)( void )
{
	return this;
}
xui_method_explain(cocos_propmaterial, was_modify,				bool					)( void )
{
	return m_modify;
}
xui_method_explain(cocos_propmaterial, load,					void					)( void )
{
	m_modify = false;

	if (get_ctrl())
	{
		cocos_inspector* inspector = cocos_mainform::get_ptr()->get_inspector();
		inspector->set_proproot(NULL);
	}

	m_name = L"default_mat";
	m_tech = L"default_tech";
	m_pass.blend		= true;
	m_pass.depthtest	= true;
	m_pass.depthwrite	= false;
	m_pass.cullface		= false;
	m_pass.blendsrc		= cocos2d::RenderState::BLEND_ONE;
	m_pass.blenddst		= cocos2d::RenderState::BLEND_ZERO;
	m_pass.depthfunc	= cocos2d::RenderState::DEPTH_LESS;
	m_pass.cullfaceside = cocos2d::RenderState::CULL_FACE_SIDE_BACK;
	m_pass.frontface	= cocos2d::RenderState::FRONT_FACE_CCW;
	m_pass.program.vertshader = NULL;
	m_pass.program.fragshader = NULL;
	xui_method_refcall(m_pass.program.vertshaderfile,	clear)();
	xui_method_refcall(m_pass.program.fragshaderfile,	clear)();
	xui_method_refcall(m_pass.program.vertuniforms,		clear)();
	xui_method_refcall(m_pass.program.fraguniforms,		clear)();
	xui_method_refcall(m_pass.program.fragsamplers,		clear)();

	if (xui_global::has_file(m_fullname))
	{
		cocos2d::Properties* root = cocos2d::Properties::createNonRefCounted(xui_global::unicode_to_ascii(m_fullname));
		if (root)
		{
			parse_material((strlen(root->getNamespace()) > 0) ? root : root->getNextNamespace());
			delete root;
		}
	}

	if (m_vertkind)
	{
		xui_propdata_vec vec = m_vertkind->get_propdata();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			cocos_propdata_uniform* datauniform = dynamic_cast<cocos_propdata_uniform*>(vec[i]);
			if (datauniform)
				datauniform->reset();
		}
	}
	if (m_fragkind)
	{
		xui_propdata_vec vec = m_fragkind->get_propdata();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			cocos_propdata_uniform* datauniform = dynamic_cast<cocos_propdata_uniform*>(vec[i]);
			if (datauniform)
				datauniform->reset();
			cocos_propdata_sampler* datasampler = dynamic_cast<cocos_propdata_sampler*>(vec[i]);
			if (datasampler)
				datasampler->reset();
		}
	}
}

/*
//event
*/
xui_method_explain(cocos_propmaterial, on_propchanged,			void					)( xui_component* sender, xui_method_propdata&	args )
{
	m_modify = true;
}
xui_method_explain(cocos_propmaterial, on_namechanged,			void					)( xui_component* sender, xui_method_args&		args )
{
	xui_textbox* textbox = xui_dynamic_cast(xui_textbox, sender);
	m_name	 = textbox->get_text();
	m_modify = true;
}

/*
//override
*/
xui_method_explain(cocos_propmaterial, save_as,					void					)( const std::wstring& fullname, bool modify )
{
	m_modify = modify;
	//TODO
	FILE* file = fopen(xui_global::unicode_to_ascii(fullname).c_str(), "w");
	if (file)
	{
		fclose(file);
	}
}

/*
//method
*/
xui_method_explain(cocos_propmaterial, parse_material,			void					)( cocos2d::Properties* root )
{
	if (root == NULL)
		return;

	m_name = xui_global::ascii_to_unicode(root->getId());
	cocos2d::Properties* next = root->getNextNamespace();
	if (next)
	{
		std::string name = next->getNamespace();
		if (name == "technique")
		{
			parse_technique(next);
		}
	}
}
xui_method_explain(cocos_propmaterial, parse_technique,			void					)( cocos2d::Properties* root )
{
	m_tech = xui_global::ascii_to_unicode(root->getId());
	cocos2d::Properties* next = root->getNextNamespace();
	if (next)
	{
		std::string name = next->getNamespace();
		if (name == "pass")
		{
			parse_pass(next);
		}
	}
}
xui_method_explain(cocos_propmaterial, parse_pass,				void					)( cocos2d::Properties* root )
{
	cocos2d::Properties* next = root->getNextNamespace();
	while (next)
	{
		std::string name = next->getNamespace();
		if		(name == "shader")		parse_program(next);
		else if (name == "renderState") parse_state  (next);

		next = root->getNextNamespace();
	}
}
xui_method_explain(cocos_propmaterial, parse_program,			void					)( cocos2d::Properties* root )
{
	m_pass.program.vertshaderfile = parse_optional(root, "vertexShader",	"");
	m_pass.program.fragshaderfile = parse_optional(root, "fragmentShader",	"");

	if (m_pass.program.vertshaderfile.length() > 0)
	{
		parse_shader(m_pass.program.vertshaderfile, &m_pass.program.vertuniforms, NULL);
		for (u32 i = 0; i < m_pass.program.vertuniforms.size(); ++i)
		{
			parse_uniform(root, &m_pass.program.vertuniforms[i]);
		}
	}
	if (m_pass.program.fragshaderfile.length() > 0)
	{
		parse_shader(m_pass.program.fragshaderfile, &m_pass.program.fraguniforms, NULL);
		for (u32 i = 0; i < m_pass.program.fraguniforms.size(); ++i)
		{
			parse_uniform(root, &m_pass.program.fraguniforms[i]);
		}

		cocos2d::Properties* next = root->getNextNamespace();
		while (next)
		{
			std::string name = next->getNamespace();
			if (name == "sampler")
			{
				cocos_propmaterial::sampler sampler_value;
				parse_sampler(next, &sampler_value);
				m_pass.program.fragsamplers.push_back(sampler_value);
			}

			next = root->getNextNamespace();
		}
	}
}
xui_method_explain(cocos_propmaterial, parse_sampler,			void					)( cocos2d::Properties* root, cocos_propmaterial::sampler* sampler_value )
{
	sampler_value->name = root->getId();

	std::string filename = root->getString("path");
	sampler_value->texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(filename);
	if (sampler_value->texture)
	{
		// mipmap
		std::string mipmap = parse_optional(root, "mipmap", "false");
		if (mipmap== "true")
			sampler_value->mipmap = true;

		std::string wrapS = parse_optional(root, "wrapS", "CLAMP_TO_EDGE");
		if (wrapS == "CLAMP_TO_EDGE")
			sampler_value->wraps = GL_CLAMP_TO_EDGE;

		std::string wrapT = parse_optional(root, "wrapT", "CLAMP_TO_EDGE");
		if (wrapT == "CLAMP_TO_EDGE")
			sampler_value->wrapt = GL_CLAMP_TO_EDGE;

		std::string minFilter = parse_optional(root, "minFilter", "NEAREST");
		if (minFilter == "LINEAR")
			sampler_value->minfilter = GL_LINEAR;
		else 
		if (minFilter == "NEAREST_MIPMAP_NEAREST")
			sampler_value->minfilter = GL_NEAREST_MIPMAP_NEAREST;
		else 
		if (minFilter == "LINEAR_MIPMAP_NEAREST")
			sampler_value->minfilter = GL_LINEAR_MIPMAP_NEAREST;
		else
		if (minFilter == "NEAREST_MIPMAP_LINEAR")
			sampler_value->minfilter = GL_NEAREST_MIPMAP_LINEAR;
		else 
		if (minFilter == "LINEAR_MIPMAP_LINEAR")
			sampler_value->minfilter = GL_LINEAR_MIPMAP_LINEAR;

		std::string magFilter = parse_optional(root, "magFilter", "NEAREST");
		if (magFilter == "LINEAR")
			sampler_value->magfilter = GL_LINEAR;
	}
}
xui_method_explain(cocos_propmaterial, parse_uniform,			void					)( cocos2d::Properties* root, cocos_propmaterial::uniform* uniform_value )
{
	const char* text = root->getString(uniform_value->name.c_str());
	if (text)
	{
		std::vector<std::wstring> vec = xui_global::get_split(xui_global::ascii_to_unicode(text), L',');
		if (vec.size() != uniform_value->data.size())
			return;

		for (u32 i = 0; i < vec.size(); ++i)
		{
			f32 value;
			std::wstringstream temp(vec[i].c_str());
			temp >> value;
			uniform_value->data[i] = value;
		}
	}
}
xui_method_explain(cocos_propmaterial, parse_state,				void					)( cocos2d::Properties* root )
{
	const char* name = root->getNextProperty();
	while (name)
	{
		std::string value = root->getString(name);
		if		(strcmp(name, "blend")			== 0)	m_pass.blend		= (value == "true");
		else if (strcmp(name, "cullFace")		== 0)	m_pass.cullface		= (value == "true");
		else if (strcmp(name, "depthTest")		== 0)	m_pass.depthtest	= (value == "true");
		else if (strcmp(name, "depthWrite")		== 0)	m_pass.depthwrite	= (value == "true");
		else if (strcmp(name, "blendSrc")		== 0)	m_pass.blendsrc		= parse_textmap(value, text_blend);
		else if (strcmp(name, "blendDst")		== 0)	m_pass.blenddst		= parse_textmap(value, text_blend);
		else if (strcmp(name, "cullFaceSide")	== 0)	m_pass.cullfaceside = parse_textmap(value, text_cullfaceside);
		else if (strcmp(name, "frontFace")		== 0)	m_pass.frontface	= parse_textmap(value, text_frontface);
		else if (strcmp(name, "depthFunc")		== 0)	m_pass.depthfunc	= parse_textmap(value, text_depth);

		name = root->getNextProperty();
	}
}
xui_method_explain(cocos_propmaterial, parse_optional,			std::string				)( cocos2d::Properties* root, const char* key, const std::string& default_value )
{
	const char* ret = root->getString(key);
	if (ret == NULL)
		return default_value;

	return ret;
}
xui_method_explain(cocos_propmaterial, parse_textmap,			s32						)( const std::string& value, const std::map<s32, std::wstring>& textmap )
{
	std::map<s32, std::wstring>::const_iterator itor = textmap.begin();
	for (; itor != textmap.end(); ++itor)
	{
		if (xui_global::unicode_to_ascii((*itor).second) == value)
			return (*itor).first;
	}

	return 0;
}
xui_method_explain(cocos_propmaterial, parse_shader,			void					)( const std::string& fullname, std::vector<cocos_propmaterial::uniform>* uniform_vec, std::vector<cocos_propmaterial::sampler>* sampler_vec )
{
	FILE* file = fopen(fullname.c_str(), "r");
	if (file)
	{
		while (feof(file) == false)
		{
			std::string line = xui_global::get_fileline(file);
			if (line.find("//") != -1 ||
				line.find("[")  != -1 ||
				line.find("]")  != -1)
				continue;

			if (line.find("uniform") != -1)
			{
				std::string name;
				std::string type;
				s32 pos;
				pos = line.find_last_of(' ');
				if (pos != -1)
				{
					name = line.substr(pos+1, line.length()-pos-2);
					line.erase(pos, line.length()-pos-1);
				}
				pos = line.find_last_of(' ');
				if (pos != -1)
				{
					type = line.substr(pos+1, line.length()-pos-2);
				}

				if (name.length() > 0 && type.length() > 0)
				{
					if (type == "sampler2D")
					{
						cocos_propmaterial::sampler value;
						value.name		= name;
						value.mipmap	= false;
						value.texture	= NULL;
						if (sampler_vec)
							sampler_vec->push_back(value);
					}
					else
					{
						cocos_propmaterial::uniform value;
						value.name = name;
						if		(type == "float")	value.data.resize(1,  0.0f);
						else if (type == "vec2")	value.data.resize(2,  0.0f);
						else if (type == "vec3")	value.data.resize(3,  0.0f);
						else if (type == "vec4")	value.data.resize(4,  0.0f);
						else if (type == "mat4")	value.data.resize(16, 0.0f);
						if (uniform_vec)
							uniform_vec->push_back(value);
					}
				}
			}
		}

		fclose(file);
	}
}

/*
//static
*/
xui_method_explain(cocos_propmaterial, get_vertshader,			void*					)( void* userptr )
{
	cocos_propmaterial* prop = (cocos_propmaterial*)userptr;
	return prop->get_vertshader();
}
xui_method_explain(cocos_propmaterial, set_vertshader,			void					)( void* userptr, void* value )
{
	cocos_propmaterial* prop = (cocos_propmaterial*)userptr;
	prop->set_vertshader((cocos_propvertshader*)value);
}
xui_method_explain(cocos_propmaterial, get_fragshader,			void*					)( void* userptr )
{
	cocos_propmaterial* prop = (cocos_propmaterial*)userptr;
	return prop->get_fragshader();
}
xui_method_explain(cocos_propmaterial, set_fragshader,			void					)( void* userptr, void* value )
{
	cocos_propmaterial* prop = (cocos_propmaterial*)userptr;
	prop->set_fragshader((cocos_propfragshader*)value);
}