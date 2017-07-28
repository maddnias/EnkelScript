#include "stdafx.h"
#include "call_visitor.h"
#include "enkel_runtime.h"

using namespace std;

namespace enkel {
	namespace runtime {

		call_visitor::call_visitor(enkel_runtime &rt,
			compiler::func_decl_node &targetFunc)
			: exec_visitor(rt),
			mTargetFunc(targetFunc),
			mRecursive(false) {
		}

		call_visitor::~call_visitor() {
		}

		void call_visitor::visit(compiler::call_expr_node &node) {
			if (node.get_target_name() == mTargetFunc.get_name() && !mRecursive) {
				// If there is a recursive call in the target function it needs
				// to be executed through the exec_visitor
				mRecursive = true;
				mTargetFunc.accept(*this);
			} else {
				exec_visitor::visit(node);
			}
		}

		bool call_visitor::has_retval() const {
			return !mRetVal.is_empty();
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