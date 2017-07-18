#pragma once
#include "base_node.h"

namespace enkel {
	namespace compiler {
		class var_decl_expr_node : public base_node {
		public:
			 var_decl_expr_node(const std::string &scopeDecl, std::string &name,
				std::unique_ptr<base_node> assignment)
				: mName(name),
				mScopeDecl(scopeDecl),
				mAssignment(move(assignment)) {
			}

			const std::string& get_name() const {
				return mName;
			}

			std::string dump() override {
				return (mScopeDecl.empty() ? "" : mScopeDecl) + mName;
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

			const std::string& get_scope_decl() const {
				return mScopeDecl;
			 }

			std::unique_ptr<base_node>& get_init_expr() {
				return mAssignment;
			 }

		private:
			std::string mName;
			std::string mScopeDecl;
			std::unique_ptr<base_node> mAssignment;
		};
	}
}
