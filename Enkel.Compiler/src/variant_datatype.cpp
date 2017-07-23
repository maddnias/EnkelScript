#include "stdafx.h"
#include "variant_datatype.h"

using namespace std;

//TODO: finish all operations
namespace enkel {
	namespace runtime {
		variant_datatype::variant_datatype()
			: mType(VAR_TYPE_STRING),
			  mStrVal(),
			  mI32Val(0),
			  mI64Val(0),
			  mUI64Val(0),
			  mFVal(0),
			  mPtrVal(nullptr),
			  mEmpty(true) {
		}

		variant_datatype::variant_datatype(wstring val)
			: mType(VAR_TYPE_STRING),
			  mStrVal(val),
			  mI32Val(0),
			  mI64Val(0),
			  mUI64Val(0),
			  mFVal(0),
			  mPtrVal(nullptr),
			  mEmpty(false) {
		}

		variant_datatype::variant_datatype(int val)
			: mType(VAR_TYPE_I32),
			  mStrVal(),
			  mI32Val(val),
			  mI64Val(0),
			  mUI64Val(0),
			  mFVal(0),
			  mPtrVal(nullptr),
			  mEmpty(false) {
		}

		variant_datatype::variant_datatype(int64_t val)
			: mType(VAR_TYPE_I64),
			  mStrVal(),
			  mI32Val(0),
			  mI64Val(val),
			  mUI64Val(0),
			  mFVal(0),
			  mPtrVal(nullptr),
			  mEmpty(false) {
		}

		variant_datatype::variant_datatype(uint64_t val)
			: mType(VAR_TYPE_UI64),
			  mStrVal(),
			  mI32Val(0),
			  mI64Val(0),
			  mUI64Val(val),
			  mFVal(0),
			  mPtrVal(nullptr),
			  mEmpty(false) {
		}

		variant_datatype::variant_datatype(double val)
			: mType(VAR_TYPE_FLOAT),
			  mStrVal(),
			  mI32Val(0),
			  mI64Val(0),
			  mUI64Val(0),
			  mFVal(val),
			  mPtrVal(nullptr),
			  mEmpty(false) {
		}

		variant_datatype::~variant_datatype() {
		}

		variant_datatype::operator bool() const {
			return mI32Val == 1
				|| mI64Val == 1
				|| mUI64Val == 1
				|| mFVal == 1
				|| mStrVal == L"1";
		}

		variant_datatype& variant_datatype::operator=(const variant_datatype &variant2) {
			mEmpty = false;
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
			mEmpty = false;
			mType = VAR_TYPE_I32;
			mI32Val = i32dat;

			return *this;
		}

		variant_datatype& variant_datatype::operator=(int64_t i64dat) {
			mEmpty = false;
			mType = VAR_TYPE_I64;
			mI64Val = i64dat;

			return *this;
		}

		variant_datatype& variant_datatype::operator=(void *ptrDat) {
			mEmpty = false;
			mType = VAR_TYPE_PTR;
			mPtrVal = ptrDat;

			return *this;
		}

		variant_datatype& variant_datatype::operator=(double floatDat) {
			mEmpty = false;
			mType = VAR_TYPE_FLOAT;
			mFVal = floatDat;

			return *this;
		}

		variant_datatype& variant_datatype::operator=(wstring strDat) {
			mEmpty = false;
			mType = VAR_TYPE_STRING;
			mStrVal = strDat;

			return *this;
		}

		variant_datatype& variant_datatype::operator+(variant_datatype &other) {
			if (!ensure_addition_possible(mType, other.get_type())) {
				//TODO: err
			}

			int64_t i64tmp;
			int i32tmp;

			switch (mType) {
			case VAR_TYPE_STRING:
				switch (other.get_type()) {
				case VAR_TYPE_STRING:
					mStrVal += other.mStrVal;
					break;
				case VAR_TYPE_I32:
					mStrVal += to_wstring(other.mI32Val);
					break;
				case VAR_TYPE_I64:
					mStrVal += to_wstring(other.mI64Val);
					break;
				case VAR_TYPE_FLOAT:
					mStrVal += to_wstring(other.mFVal);
					break;
				case VAR_TYPE_PTR: break;
				default: ;
				}
				break;

			case VAR_TYPE_I32:
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
						//TODO: check this out
						mStrVal = to_wstring(mI32Val) + other.mStrVal;
					}
					break;

				case VAR_TYPE_I32:
					i32tmp = mI32Val + other.mI32Val;
					i64tmp = static_cast<int64_t>(mI32Val) + static_cast<int64_t>(other.mI32Val);

					// Check if i32 overflow
					if (static_cast<int64_t>(i32tmp) != i64tmp) {
						reset_var(VAR_TYPE_I64);
						mI64Val = i64tmp;
					}
					else {
						mI32Val = i32tmp;
					}
					break;

				case VAR_TYPE_I64:
					reset_var(VAR_TYPE_I64);
					mI64Val = static_cast<int64_t>(mI32Val) + other.mI64Val;
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
						reset_var(VAR_TYPE_STRING);
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

			case VAR_TYPE_FLOAT: break;
			case VAR_TYPE_PTR: break;
			default: ;
			}

			return *this;
		}

		//TODO: UI64
		variant_datatype& variant_datatype::operator*(variant_datatype &other) {
			int64_t i64tmp;
			int i32tmp;

			switch (mType) {
			case VAR_TYPE_STRING:
				//TODO: str * str
				break;

			case VAR_TYPE_I32:
				switch (other.get_type()) {
				case VAR_TYPE_STRING:
					//TODO: str * (*)
					break;

				case VAR_TYPE_I32:
					i32tmp = mI32Val * other.mI32Val;
					i64tmp = static_cast<int64_t>(mI32Val) * static_cast<int64_t>(other.mI32Val);

					// Check if i32 overflow
					if (static_cast<int64_t>(i32tmp) != i64tmp) {
						reset_var(VAR_TYPE_I64);
						mI64Val = i64tmp;
					}
					else {
						mI32Val = i32tmp;
					}
					break;

				case VAR_TYPE_I64:
					reset_var(VAR_TYPE_I64);
					mI64Val = static_cast<int64_t>(mI32Val) * other.mI64Val;
					break;

				case VAR_TYPE_FLOAT: break;
				case VAR_TYPE_PTR: break;
				default: ;
				}
				break;
			case VAR_TYPE_I64: break;
			case VAR_TYPE_UI64: break;
			case VAR_TYPE_FLOAT: break;
			}

			return *this;
		}

		//TODO: ptr
		bool variant_datatype::operator==(variant_datatype &other) const {
			switch (mType) {
			case VAR_TYPE_STRING:
				return mStrVal == other.mStrVal;
			case VAR_TYPE_I32:
				return mI32Val == other.mI32Val;
			case VAR_TYPE_I64:
				return mI64Val == other.mI64Val;
			case VAR_TYPE_UI64:
				return mUI64Val == other.mUI64Val;
			case VAR_TYPE_FLOAT:
				return mFVal == other.mFVal;
			default:
				return false;;
			}
		}

		bool variant_datatype::operator!=(variant_datatype &other) const {
			return !(*this == other);
		}

		variant_datatype::var_type variant_datatype::get_type() const {
			return mType;
		}

		double variant_datatype::val_as_double() const {
			switch (mType) {
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

		void variant_datatype::reset_var(var_type type) {
			mType = type;
			mStrVal = L"";
			mI32Val = 0;
			mI64Val = 0;
			mUI64Val = 0;
			mFVal = 0.0f;
			//TODO: ptr
		}

		bool variant_datatype::ensure_addition_possible(var_type lType, var_type rType) {
			switch (lType) {
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
			switch (mType) {

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

		bool variant_datatype::empty() const {
			return mEmpty;
		}

		wostream& operator<<(wostream &os, const variant_datatype &var) {
			wstring strData;
			if (!var.ensure_type(variant_datatype::VAR_TYPE_STRING)) {
				strData = var.to_string();
			}
			else {
				strData = L"\"" + var.mStrVal + L"\"";
			}
			os << strData;
			return os;
		}
	}
}
