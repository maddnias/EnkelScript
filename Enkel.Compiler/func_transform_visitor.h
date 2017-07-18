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
			func_transform_visitor();
			~func_transform_visitor();
			module_node& visit(module_node &node) override;
			module_elem_node& visit(module_elem_node &node) override;
			block_node& visit(block_node &node) override;
			param_node& visit(param_node &node) override;
			assignment_stmt_node& visit(assignment_stmt_node &node) override;
			assign_expr_node& visit(assign_expr_node &node) override;
			bin_expr_node& visit(bin_expr_node &node) override;
			func_decl_node& visit(func_decl_node &node) override;
			func_node& visit(func_node &node) override;
			number_expr_node& visit(number_expr_node &node) override;
			return_expr_node& visit(return_expr_node &node) override;
			var_decl_expr_node& visit(var_decl_expr_node &node) override;
			var_expr_node& visit(var_expr_node &node) override;
			param_list_node& visit(param_list_node &node) override;
			call_expr_node& visit(call_expr_node &node) override;

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

