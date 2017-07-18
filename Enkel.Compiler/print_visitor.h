#pragma once
#include "base_ast_visitor.h"
#include <ostream>
#include <string>

#define NL "\n"


namespace enkel {
	namespace compiler {
		class print_visitor :
			public base_ast_visitor {
		public:
			print_visitor(std::ostream &outStream);
			~print_visitor();

			module_node& visit(module_node &node) override;
			module_elem_node& visit(module_elem_node &node) override;
			block_node& visit(block_node &node) override;
			param_node& visit(param_node &node) override;
			assignment_stmt_node& visit(assignment_stmt_node &node) override;
			assign_expr_node& visit(assign_expr_node &node) override;
			bin_expr_node& visit(bin_expr_node &node) override;
			call_expr_node& visit(call_expr_node &node) override;
			func_decl_node& visit(func_decl_node &node) override;
			func_node& visit(func_node &node) override;
			number_expr_node& visit(number_expr_node &node) override;
			return_expr_node& visit(return_expr_node &node) override;
			var_decl_expr_node& visit(var_decl_expr_node &node) override;
			var_expr_node& visit(var_expr_node &node) override;
			param_list_node& visit(param_list_node &node) override;

		private:
			template<typename... Args /* why different name required?? */>
			void print(Args&&... args) const;
			std::string get_indent() const;
			int reset_indent();
			void indent();
			void de_indent();

			std::ostream &mOutStream;
			int mIndent;
		};


		template <typename... Args>
		void print_visitor::print(Args&&... args) const {
			mOutStream << get_indent();
			using expander = int[];
			(void)expander {
				0, (void(mOutStream << std::forward<Args>(args)), 0) ...
			};
		}
	}
}
