#pragma once
#include "base_ast_visitor.h"
#include <vector>
#include "base_node.h"

namespace enkel {
	namespace runtime {
		class enkel_runtime;

		class exec_visitor :
			public compiler::base_ast_visitor {
		public:
			exec_visitor(enkel_runtime &rt);
			~exec_visitor();
			compiler::module_node& visit(compiler::module_node &node) override;
			compiler::module_elem_node& visit(compiler::module_elem_node &node) override;
			compiler::block_node& visit(compiler::block_node &node) override;
			compiler::param_node& visit(compiler::param_node &node) override;
			compiler::assignment_stmt_node& visit(compiler::assignment_stmt_node &node) override;
			compiler::assign_expr_node& visit(compiler::assign_expr_node &node) override;
			compiler::bin_expr_node& visit(compiler::bin_expr_node &node) override;
			compiler::func_decl_node& visit(compiler::func_decl_node &node) override;
			compiler::func_node& visit(compiler::func_node &node) override;
			compiler::number_expr_node& visit(compiler::number_expr_node &node) override;
			compiler::return_expr_node& visit(compiler::return_expr_node &node) override;
			compiler::var_decl_expr_node& visit(compiler::var_decl_expr_node &node) override;
			compiler::var_expr_node& visit(compiler::var_expr_node &node) override;
			compiler::param_list_node& visit(compiler::param_list_node &node) override;
			compiler::call_expr_node& visit(compiler::call_expr_node &node) override;
		private:
			static bool ensure_arg_count(std::vector<std::unique_ptr<compiler::base_node>> &args, int expectedCount);
			enkel_runtime& mRuntime;
		};
	}
}
