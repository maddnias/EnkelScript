#pragma once
#include "base_node.h"
#include "block_node.h"
#include "param_list_node.h"

namespace enkel {
	namespace compiler {
		class func_decl_node : public expr_node {
		public:

			func_decl_node(const std::wstring &name, std::unique_ptr<param_list_node> params,
				std::unique_ptr<block_node> body)
				: mName(name),
				mParamList(move(params)),
				mBody(move(body)) {
			}

			std::wstring get_name() const {
				return mName;
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

			std::unique_ptr<param_list_node>& get_params() {
				return mParamList;
			}

			std::unique_ptr<block_node>& get_body() {
				return mBody;
			}

		private:
			std::wstring mName;
			std::unique_ptr<param_list_node> mParamList;
			std::unique_ptr<block_node> mBody;

		};
	}
}
