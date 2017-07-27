#pragma once
#include "base_node.h"

namespace enkel {
	namespace compiler {
		class var_decl_expr_node : public base_node {
		public:
			 var_decl_expr_node(const std::wstring &scopeDecl, const std::wstring &name,
				std::unique_ptr<base_node> assignment)
				: mName(name),
				mScopeDecl(scopeDecl),
				mAssignment(move(assignment)) {
			}

			const std::wstring& get_name() const {
				return mName;
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

			const std::wstring& get_scope_decl() const {
				return mScopeDecl;
			 }

			std::unique_ptr<base_node>& get_init_expr() {
				return mAssignment;
			 }

		private:
			std::wstring mName;
			std::wstring mScopeDecl;
			std::unique_ptr<base_node> mAssignment;
		};
	}
}
