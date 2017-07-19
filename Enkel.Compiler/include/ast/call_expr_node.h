#pragma once
#include "enkel_stl.h"
#include "expr_node.h"
#include <vector>

namespace enkel {
	namespace compiler {
		class call_expr_node
			: public expr_node {
		public:
			call_expr_node(std::wstring &target)
				: mIsStl(enkel_stl::is_stl_func(target)),
				  mTargetFunc(target) {
			}

			void add_arg(std::unique_ptr<base_node> arg) {
				mArgs.push_back(move(arg));
			}

			std::vector<std::unique_ptr<base_node>>& get_args() {
				return mArgs;
			}

			bool get_isstl() const {
				return mIsStl;
			}

			std::wstring& get_target_name() {
				return mTargetFunc;
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

		private:
			bool mIsStl;
			std::wstring mTargetFunc;
			std::vector<std::unique_ptr<base_node>> mArgs;
		};
	}
}
