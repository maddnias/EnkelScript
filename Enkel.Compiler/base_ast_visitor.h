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
		class const_expr_node;
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

			virtual void visit(module_node &node) {}
			virtual void visit(module_elem_node &node) {}
			virtual void visit(block_node &node) {}
			virtual void visit(param_node &node){}
			virtual void visit(assignment_stmt_node &node) {}
			virtual void visit(assign_expr_node &node) {}
			virtual void visit(bin_expr_node &node) {}
			virtual void visit(call_expr_node &node) {}
			virtual void visit(func_decl_node &node) {}
			virtual void visit(func_node &node) {}
			virtual void visit(const_expr_node &node) {}
			virtual void visit(return_expr_node &node) {}
			virtual void visit(var_decl_expr_node &node) {}
			virtual void visit(var_expr_node &node) {}
			virtual void visit(param_list_node &node) {}
		};
	}
}
