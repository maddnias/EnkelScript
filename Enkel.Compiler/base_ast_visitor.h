#pragma once
#include <memory>

namespace enkel {
	namespace compiler {
		class param_list_node;
		class module_elem_node;
		class assignment_stmt_node;
		class param_node;
		class module_node;
		class var_expr_node;
		class var_decl_expr_node;
		class return_expr_node;
		class number_expr_node;
		class func_node;
		class func_decl_node;
		class bin_expr_node;
		class assign_expr_node;
		class block_node;
		class call_expr_node;

		class base_ast_visitor
		{
		public:
			base_ast_visitor() {}
			virtual ~base_ast_visitor() {}

			virtual module_node& visit(module_node &node) = 0;
			virtual module_elem_node& visit(module_elem_node &node) = 0;
			virtual block_node& visit(block_node &node) = 0;
			virtual param_node& visit(param_node &node) = 0;
			virtual assignment_stmt_node& visit(assignment_stmt_node &node) = 0;
			virtual assign_expr_node& visit(assign_expr_node &node) = 0;
			virtual bin_expr_node& visit(bin_expr_node &node) = 0;
			virtual call_expr_node& visit(call_expr_node &node) = 0;
			virtual func_decl_node& visit(func_decl_node &node) = 0;
			virtual func_node& visit(func_node &node) = 0;
			virtual number_expr_node& visit(number_expr_node &node) = 0;
			virtual return_expr_node& visit(return_expr_node &node) = 0;
			virtual var_decl_expr_node& visit(var_decl_expr_node &node) = 0;
			virtual var_expr_node& visit(var_expr_node &node) = 0;
			virtual param_list_node& visit(param_list_node &node) = 0;
		};
	}
}
