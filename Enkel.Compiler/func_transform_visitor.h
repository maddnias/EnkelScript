#pragma once
#include "base_ast_visitor.h"
#include <vector>
#include <memory>
#include "func_decl_node.h"

namespace enkel {
	namespace compiler {
		class func_transform_visitor
			: public base_ast_visitor
		{
		public:
			void visit(module_node &node) override;
			void visit(module_elem_node &node) override;
	
			/**
			 * \brief Returns and transfers ownership of the vector of function declarations
			 * \return Vector of function declarations
			 */
			std::vector<std::unique_ptr<func_decl_node>> get_decls();

			
		private:
			std::vector<std::unique_ptr<func_decl_node>> mFuncDecls;
		};
	}
}

