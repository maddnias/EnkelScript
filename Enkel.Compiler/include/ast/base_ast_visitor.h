#pragma once

namespace enkel {
	namespace compiler {
			class base_ast_visitor
		{
		public:
			base_ast_visitor() {}
			virtual ~base_ast_visitor() {}

			virtual void visit(class module_node &node) {}
			virtual void visit(class module_elem_node &node) {}
			virtual void visit(class block_node &node) {}
			virtual void visit(class param_node &node){}
			virtual void visit(class assignment_stmt_node &node) {}
			virtual void visit(class assign_expr_node &node) {}
			virtual void visit(class bin_expr_node &node) {}
			virtual void visit(class call_expr_node &node) {}
			virtual void visit(class func_decl_node &node) {}
			virtual void visit(class func_node &node) {}
			virtual void visit(class const_expr_node &node) {}
			virtual void visit(class return_expr_node &node) {}
			virtual void visit(class var_decl_expr_node &node) {}
			virtual void visit(class var_expr_node &node) {}
			virtual void visit(class param_list_node &node) {}
			virtual void visit(class if_stmt_node &node) {}
			virtual void visit(class loop_stmt_node &node) {}
		};
	}
}
