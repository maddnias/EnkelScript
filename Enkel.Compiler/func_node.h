#pragma once
#include "func_decl_node.h"
#include "base_node.h"
#include <memory>

namespace enkel {
	namespace compiler {
		class func_node : public base_node {
		public:
			func_node(std::unique_ptr<func_decl_node> decl,
			          std::unique_ptr<base_node> funcBody)
				: mDecl(move(decl)),
				  mBody(move(funcBody)) {

			}

			const func_decl_node &get_decl() const {
				return *mDecl;
			}

			base_node &get_body() const {
				return *mBody;
			}

			std::wstring get_name() {
				return mDecl->get_name();
			}

			void accept(base_ast_visitor &visitor) override {}

		private:
			std::unique_ptr<func_decl_node> mDecl;
			std::unique_ptr<base_node> mBody;
		};
	}
}
