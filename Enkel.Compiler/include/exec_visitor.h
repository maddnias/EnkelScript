#pragma once
#include "nodes.h"
#include <vector>

namespace enkel {
	namespace runtime {
		class enkel_runtime;

		class exec_visitor :
			public compiler::base_ast_visitor {
		public:
			exec_visitor(enkel_runtime &rt);
			~exec_visitor();
			void visit(compiler::module_node &node) override;
			void visit(compiler::module_elem_node &node) override;
			void visit(compiler::bin_expr_node &node) override;
			void visit(compiler::return_expr_node &node) override;
			void visit(compiler::var_decl_expr_node &node) override;
			void visit(compiler::var_expr_node &node) override;
			void visit(compiler::call_expr_node &node) override;
			void visit(compiler::if_stmt_node &node) override;
			void visit(compiler::block_node &node) override;
			void visit(compiler::func_decl_node &node) override;

		private:
			static bool ensure_arg_count(std::vector<std::unique_ptr<compiler::base_node>> &args, int expectedCount);
			enkel_runtime& mRuntime;
		};
	}
}
