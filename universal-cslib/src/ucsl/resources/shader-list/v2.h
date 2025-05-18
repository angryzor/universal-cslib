#pragma once

namespace ucsl::resources::shader_list::v2 {
	struct ShaderListParameterData {
		const char* name;
		unsigned int id;
	};

	struct ShaderListShaderData {
		const char* type;
		const char* name;
	};

	struct ShaderListTableData {
		ShaderListParameterData* parameters;
		unsigned int parameterCount;
		ShaderListShaderData* shaders;
		unsigned int shaderCount;
	};

	struct ShaderListData {
		unsigned int magic;
		unsigned int version;
		ShaderListTableData table;
	};
}
