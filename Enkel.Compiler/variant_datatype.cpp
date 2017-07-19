#include "stdafx.h"
#include "variant_datatype.h"
#include <cassert>

using namespace std;

namespace enkel {
	namespace runtime {
		variant_datatype::variant_datatype() {
		}

		variant_datatype::variant_datatype(wstring val)
			: mType(VAR_TYPE_STRING),
			mStrVal(val),
			mI32Val(0),
			mI64Val(0),
			mFVal(0),
			mPtrVal(nullptr) {
		}

		variant_datatype::variant_datatype(int val)
			: mType(VAR_TYPE_I32),
			mStrVal(),
			mI32Val(val),
			mI64Val(0),
			mFVal(0),
			mPtrVal(nullptr) {
		}

		variant_datatype::~variant_datatype() {
		}

		variant_datatype& variant_datatype::operator=(const variant_datatype &variant2) {
			mType = variant2.get_type();
			switch (mType) {
			case VAR_TYPE_STRING:
				mStrVal = variant2.mStrVal;
				break;
			case VAR_TYPE_I32:
				mI32Val = variant2.mI32Val;
				break;
			case VAR_TYPE_I64:
				mI64Val = variant2.mI64Val;
				break;
			case VAR_TYPE_FLOAT:
				mFVal = variant2.mFVal;
				break;
			case VAR_TYPE_PTR:
				mPtrVal = variant2.mPtrVal;
				break;
			default:
				//TODO: throw error
				;
			}

			return *this;
		}

		variant_datatype& variant_datatype::operator=(int i32dat) {
			mType = VAR_TYPE_I32;
			mI32Val = i32dat;

			return *this;
		}

		variant_datatype& variant_datatype::operator=(__int64 i64dat) {
			mType = VAR_TYPE_I64;
			mI64Val = i64dat;

			return *this;
		}

		variant_datatype& variant_datatype::operator=(void *ptrDat) {
			mType = VAR_TYPE_PTR;
			mPtrVal = ptrDat;

			return *this;
		}

		variant_datatype& variant_datatype::operator=(double floatDat) {
			mType = VAR_TYPE_FLOAT;
			mFVal = floatDat;

			return *this;
		}

		variant_datatype& variant_datatype::operator=(wstring strDat) {
			mType = VAR_TYPE_STRING;
			mStrVal = strDat;

			return *this;
		}

		variant_datatype& variant_datatype::operator+(variant_datatype &other) {
			if(!ensure_addition_possible(mType, other.get_type())) {
				//TODO: err
			}

			switch(mType) {
			case VAR_TYPE_STRING: 
				switch(other.get_type()) {
				case VAR_TYPE_STRING: 
					mStrVal += other.mStrVal;
					break;
				case VAR_TYPE_I32: 
				case VAR_TYPE_I64:
				case VAR_TYPE_FLOAT:
					mStrVal += to_wstring(other.val_as_double());
					break;
				case VAR_TYPE_PTR: break;
				default: ;
				}
				break;
			case VAR_TYPE_I32: 
				switch(other.get_type()) {
				case VAR_TYPE_STRING: 
					wchar_t *end;
					long val;
					if((val = wcstol(other.mStrVal.c_str(), &end, 10))) {
						//TODO: check
						mI32Val += val;
					} else {
						mType = VAR_TYPE_STRING;
						reset_var();
						mStrVal = to_wstring(mI32Val) + other.mStrVal;
					}
					break;
				case VAR_TYPE_I32:
				case VAR_TYPE_I64:
				case VAR_TYPE_FLOAT: 
					mI32Val += other.val_as_double();
					break;
				case VAR_TYPE_PTR: break;
				default: ;
				}
				break;
			case VAR_TYPE_I64: 
				switch (other.get_type()) {
				case VAR_TYPE_STRING:
					wchar_t *end;
					long val;
					if ((val = wcstol(other.mStrVal.c_str(), &end, 10))) {
						//TODO: check
						mI32Val += val;
					}
					else {
						mType = VAR_TYPE_STRING;
						reset_var();
						mStrVal = to_wstring(mI32Val) + other.mStrVal;
					}
					break;
				case VAR_TYPE_I32:
				case VAR_TYPE_I64:
				case VAR_TYPE_FLOAT:
					mI32Val += other.val_as_double();
					break;
				case VAR_TYPE_PTR: break;
				default:;
				}
				break;
			case VAR_TYPE_FLOAT: break;
			case VAR_TYPE_PTR: break;
			default: ;
			}

			return *this;
		}

		variant_datatype::var_type variant_datatype::get_type() const {
			return mType;
		}

		double variant_datatype::val_as_double() const {
			switch(mType) {
			case VAR_TYPE_STRING: 
				wchar_t *end;
				return wcstod(mStrVal.c_str(), &end);
			case VAR_TYPE_I32: 
				return static_cast<double>(mI32Val);
			case VAR_TYPE_I64: 
				return static_cast<double>(mI64Val);
			case VAR_TYPE_FLOAT:
				return mFVal;
			case VAR_TYPE_PTR: break;
			default: ;
			}
			return 0.0f;
		}

		void variant_datatype::reset_var() {
			mStrVal = L"";
			mI32Val = 0;
			mI64Val = 0;
			mFVal = 0.0f;
			//TODO: ptr
		}

		bool variant_datatype::ensure_addition_possible(var_type lType, var_type rType) {
			switch(lType) {
			case VAR_TYPE_STRING: 
				return rType == VAR_TYPE_STRING
					|| rType == VAR_TYPE_I32
					|| rType == VAR_TYPE_I64
					|| rType == VAR_TYPE_FLOAT;
			case VAR_TYPE_I32: 
				return rType == VAR_TYPE_STRING
					|| rType == VAR_TYPE_I32
					|| rType == VAR_TYPE_I64
					|| rType == VAR_TYPE_FLOAT;
			case VAR_TYPE_I64: 
				return rType == VAR_TYPE_STRING
					|| rType == VAR_TYPE_I32
					|| rType == VAR_TYPE_I64
					|| rType == VAR_TYPE_FLOAT;
			case VAR_TYPE_FLOAT:
				return rType == VAR_TYPE_STRING
					|| rType == VAR_TYPE_I32
					|| rType == VAR_TYPE_I64
					|| rType == VAR_TYPE_FLOAT; 
			case VAR_TYPE_PTR: break;
			}

			return false;
		}

		bool variant_datatype::ensure_type(var_type type) const {
			return mType == type;
		}

		wstring variant_datatype::to_string() const {
			switch(mType) {

			case VAR_TYPE_STRING: 
				return mStrVal;
			case VAR_TYPE_I32:
				return to_wstring(mI32Val);
			case VAR_TYPE_I64:
				return to_wstring(mI64Val);
			case VAR_TYPE_FLOAT: 
				return to_wstring(mFVal);
			case VAR_TYPE_PTR: 
				return L"";
			default: 
				return L"";
			}
		}

		wostream & operator<<(wostream &os, const variant_datatype &var) {
			wstring strData;
			if(!var.ensure_type(variant_datatype::VAR_TYPE_STRING)) {
				strData = var.to_string();
			} else {
				strData = L"\"" + var.mStrVal + L"\"";
			}
			os << strData;
			return os;
		}
	}
}
