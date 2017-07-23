#include "stdafx.h"
#include "call_visitor.h"
#include "enkel_runtime.h"

using namespace std;

namespace enkel {
	namespace runtime {

		call_visitor::call_visitor(enkel_runtime &rt,
			shared_ptr<compiler::func_decl_node> targetFunc)
			: exec_visitor(rt),
			mTargetFunc(targetFunc) {
		}

		call_visitor::~call_visitor() {
		}

		void call_visitor::visit(compiler::call_expr_node &node) {
			mTargetFunc->accept(*this);
		}

		bool call_visitor::has_retval() const {
			return !mRetVal.empty();
		}

		variant_datatype call_visitor::get_retval() const {
			return mRetVal;
		}

		void call_visitor::visit(compiler::block_node &node) {
			for(auto &elem : node.get_statements()) {
				elem->accept(*this);
				compiler::return_expr_node *retExpr;
				if((retExpr = dynamic_cast<compiler::return_expr_node*>(elem.get()))) {
					mRetVal = retExpr->get_val();
					break;
				}
			}
		}
	}
}