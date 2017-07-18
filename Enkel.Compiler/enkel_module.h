#pragma once
#include <unordered_map>
#include "func_node.h"
#include <stack>

namespace enkel {
	namespace runtime {
		class enkel_module
		{
		public:
			typedef std::unordered_map<std::string, std::unique_ptr<compiler::func_decl_node>> FuncTable;

			enkel_module();
			~enkel_module();

			bool func_exists(std::string &name);
			void add_func(std::unique_ptr<compiler::func_decl_node> func);

			/**
			 * \brief Resolves a func_node from a name. Function must exist in table or an error occurs. Use get_or_add_func if unsure.
			 * \param name The name of the function to resolve
			 * \return A shared pointer to the resolved func_node
			 */
			std::unique_ptr<compiler::func_decl_node>& resolve_func(std::string &name);

		private:
			FuncTable mFuncTbl;
		};
	}
}
