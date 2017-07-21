#pragma once
#include "nodes.h"
#include <ostream>
#include <string>

#define NL L"\n"

namespace enkel {
	namespace compiler {
		class print_visitor :
			public base_ast_visitor {
		public:
			print_visitor(std::wostream &outStream);
			~print_visitor();

			void visit(module_node &node) override;
			void visit(module_elem_node &node) override;
			void visit(block_node &node) override;
			void visit(param_node &node) override;
			void visit(assignment_stmt_node &node) override;
			void visit(assign_expr_node &node) override;
			void visit(bin_expr_node &node) override;
			void visit(call_expr_node &node) override;
			void visit(func_decl_node &node) override;
			void visit(func_node &node) override;
			void visit(const_expr_node &node) override;
			void visit(return_expr_node &node) override;
			void visit(var_decl_expr_node &node) override;
			void visit(var_expr_node &node) override;
			void visit(param_list_node &node) override;
			void visit(if_stmt_node &node) override;

		private:
			template<typename... Args /* why different name required?? */>
			void print(Args&&... args) const;
			std::wstring get_indent() const;
			int reset_indent();
			void indent();
			void de_indent();

			std::wostream &mOutStream;
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
