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
			: mType(VAR_TYPE_DOUBLE),
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
			if (!variant2.is_empty()) {
				mEmpty = false;
			}
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
			case VAR_TYPE_DOUBLE:
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
			mType = VAR_TYPE_DOUBLE;
			mFVal = floatDat;

			return *this;
		}

		variant_datatype& variant_datatype::operator=(wstring strDat) {
			mEmpty = false;
			mType = VAR_TYPE_STRING;
			mStrVal = strDat;

			return *this;
		}

		bool variant_datatype::operator<(variant_datatype &other) const {
			switch (get_comp_type(mType, other.get_type())) {
			case VAR_TYPE_STRING:
				return wcscmp(mStrVal.c_str(), other.mStrVal.c_str()) < 0;

			case VAR_TYPE_I32:
				return val_as_i32() < other.val_as_i32();

			case VAR_TYPE_I64:
				return val_as_i64() < other.val_as_i64();

			case VAR_TYPE_DOUBLE:
				return val_as_double() < other.val_as_double();
			}

			//TODO: handle all
			return false;
		}

		bool variant_datatype::operator<=(variant_datatype &other) const {
			return !(*this > other);
		}

		bool variant_datatype::operator>(variant_datatype &other) const {
			switch (get_comp_type(mType, other.get_type())) {
			case VAR_TYPE_STRING:
				return wcscmp(mStrVal.c_str(), other.mStrVal.c_str()) > 0;

			case VAR_TYPE_I32:
				return val_as_i32() > other.val_as_i32();

			case VAR_TYPE_I64:
				return val_as_i64() > other.val_as_i64();

			case VAR_TYPE_DOUBLE:
				return val_as_double() > other.val_as_double();
			}

			//TODO: handle all
			return false;
		}

		bool variant_datatype::operator>=(variant_datatype &other) const {
			return !(*this < other);
		}

		bool variant_datatype::operator>=(const int &rhs) const {
			variant_datatype comp(rhs);
			return *this >= comp;
		}

		bool variant_datatype::operator<(const int &rhs) const {
			variant_datatype comp(rhs);
			return *this < comp;
		}

		bool variant_datatype::is_num() const {
			return mType == VAR_TYPE_I32
				|| mType == VAR_TYPE_I64
				|| mType == VAR_TYPE_DOUBLE;
		}

		variant_datatype& variant_datatype::operator+(variant_datatype &other) {
			int64_t i64tmp;
			int i32tmp;

			switch (mType) {
			case VAR_TYPE_STRING:
				mStrVal += other.val_as_string();
				break;

			case VAR_TYPE_I32:
				if (other.mType == VAR_TYPE_I32) {
					i32tmp = mI32Val + other.mI32Val;
					i64tmp = static_cast<int64_t>(mI32Val) + static_cast<int64_t>(other.mI32Val);
					if (static_cast<int64_t>(i32tmp) != i64tmp) {
						// I32 overflow
						reset_var(VAR_TYPE_I64);
						mI64Val = i64tmp;
					}
					else {
						mI32Val = i32tmp;
					}
				} 
				else if(other.mType == VAR_TYPE_I64) {
					// Always convert to I64 here
					i64tmp = static_cast<int64_t>(mI32Val) + static_cast<int64_t>(other.mI32Val);
					reset_var(VAR_TYPE_I64);
					mI64Val = i64tmp;
				} 
				else if(other.mType == VAR_TYPE_STRING) {
					auto tmpStr = val_as_string() + other.val_as_string();
					reset_var(VAR_TYPE_STRING);
					mStrVal = tmpStr;
				}
				else {
					change_to_double();
					mFVal += other.val_as_double();
				}
				break;

			case VAR_TYPE_I64:
				if(other.get_type() == VAR_TYPE_I32 || other.get_type() == VAR_TYPE_I64) {
					mI64Val += other.val_as_i64();
				} 
				else if(other.get_type() == VAR_TYPE_DOUBLE) {
					change_to_double();
					mFVal += other.val_as_double();
				} else {
					auto tmpStr = val_as_string() + other.val_as_string();
					reset_var(VAR_TYPE_STRING);
					mStrVal = tmpStr;
				}
				break;

			case VAR_TYPE_DOUBLE:
				if (other.is_num()) {
					mFVal += other.val_as_double();
				} else {
					auto tmpStr = val_as_string() + other.val_as_string();
					reset_var(VAR_TYPE_STRING);
					mStrVal = tmpStr;
				}
				break;
			}

			return *this;
		}

		variant_datatype& variant_datatype::operator+=(variant_datatype &other) {
			*this = *this + other;
			return *this;
		}

		variant_datatype& variant_datatype::operator*=(variant_datatype &other) {
			*this *= other;
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

				case VAR_TYPE_DOUBLE: break;
				case VAR_TYPE_PTR: break;
				default: ;
				}
				break;
			case VAR_TYPE_I64: break;
			case VAR_TYPE_UI64: break;
			case VAR_TYPE_DOUBLE: break;
			}

			return *this;
		}

		//TODO: ptr
		bool variant_datatype::operator==(variant_datatype &other) const {
			switch (get_comp_type(mType, other.get_type())) {
			case VAR_TYPE_STRING:
				return wcscmp(mStrVal.c_str(), other.mStrVal.c_str()) == 0;

			case VAR_TYPE_I32:
				return val_as_i32() == other.val_as_i32();

			case VAR_TYPE_I64:
				return val_as_i64() == other.val_as_i64();

			case VAR_TYPE_DOUBLE:
				return val_as_double() == other.val_as_double();
			}

			//TODO: handle all
			return false;
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
			case VAR_TYPE_DOUBLE:
				return mFVal;
			case VAR_TYPE_PTR: break;
			default: ;
			}
			return 0.0f;
		}

		void variant_datatype::change_to_i32() {
			if (mType == VAR_TYPE_I32) {
				return;
			}

			int i32Tmp = val_as_i32();
			reset_var(VAR_TYPE_I32);
			mI32Val = i32Tmp;
		}

		void variant_datatype::change_to_i64() {
			if (mType == VAR_TYPE_I64) {
				return;
			}

			int64_t i64Tmp = val_as_i64();
			reset_var(VAR_TYPE_I64);
			mI64Val = i64Tmp;
		}

		void variant_datatype::change_to_str() {
			if (mType == VAR_TYPE_STRING) {
				return;
			}

			wstring strTmp = val_as_string();
			reset_var(VAR_TYPE_STRING);
			mStrVal = strTmp;
		}

		void variant_datatype::change_to_double() {
			if(mType == VAR_TYPE_DOUBLE) {
				return;
			}

			double fTmp = val_as_double();
			reset_var(VAR_TYPE_DOUBLE);
			mFVal = fTmp;
		}

		variant_datatype variant_datatype::empty() {
			return variant_datatype();
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
					|| rType == VAR_TYPE_DOUBLE;
			case VAR_TYPE_I32:
				return rType == VAR_TYPE_STRING
					|| rType == VAR_TYPE_I32
					|| rType == VAR_TYPE_I64
					|| rType == VAR_TYPE_DOUBLE;
			case VAR_TYPE_I64:
				return rType == VAR_TYPE_STRING
					|| rType == VAR_TYPE_I32
					|| rType == VAR_TYPE_I64
					|| rType == VAR_TYPE_DOUBLE;
			case VAR_TYPE_DOUBLE:
				return rType == VAR_TYPE_STRING
					|| rType == VAR_TYPE_I32
					|| rType == VAR_TYPE_I64
					|| rType == VAR_TYPE_DOUBLE;
			case VAR_TYPE_PTR: break;
			}

			return false;
		}

		bool variant_datatype::ensure_type(var_type type) const {
			return mType == type;
		}

		wstring variant_datatype::val_as_string() const {
			switch (mType) {

			case VAR_TYPE_STRING:
				return mStrVal;
			case VAR_TYPE_I32:
				return to_wstring(mI32Val);
			case VAR_TYPE_I64:
				return to_wstring(mI64Val);
			case VAR_TYPE_DOUBLE:
				return to_wstring(mFVal);
			case VAR_TYPE_PTR:
				return L"";
			default:
				return L"";
			}
		}

		bool variant_datatype::is_empty() const {
			return mEmpty;
		}

		variant_datatype::var_type variant_datatype::get_comp_type(var_type ty1, var_type ty2) {
			switch (ty1) {
			case VAR_TYPE_STRING:
				switch (ty2) {
				case VAR_TYPE_STRING:
					return VAR_TYPE_STRING;

				case VAR_TYPE_I32:
				case VAR_TYPE_I64:
				case VAR_TYPE_DOUBLE:
					return VAR_TYPE_DOUBLE;

				case VAR_TYPE_UI64:
					return VAR_TYPE_UI64;
				}

			case VAR_TYPE_I32:
				switch (ty2) {
				case VAR_TYPE_STRING:
					//TODO: check this
					return VAR_TYPE_DOUBLE;

				case VAR_TYPE_I32:
					return VAR_TYPE_I32;

				case VAR_TYPE_I64:
					return VAR_TYPE_I64;

				case VAR_TYPE_UI64:
					return VAR_TYPE_UI64;

				case VAR_TYPE_DOUBLE:
					return VAR_TYPE_DOUBLE;
				}
				break;

			case VAR_TYPE_I64:
				switch (ty2) {
				case VAR_TYPE_STRING:
					return VAR_TYPE_DOUBLE;

				case VAR_TYPE_I32:
				case VAR_TYPE_I64:
					return VAR_TYPE_I64;

				case VAR_TYPE_UI64:
					//TODO: can't do this
					return VAR_TYPE_I64;

				case VAR_TYPE_DOUBLE:
					return VAR_TYPE_DOUBLE;
				}
				break;

			case VAR_TYPE_DOUBLE:
				switch(ty2) {

				case VAR_TYPE_STRING: 
					return VAR_TYPE_DOUBLE;

				case VAR_TYPE_I32:
				case VAR_TYPE_I64:
				case VAR_TYPE_DOUBLE: 
					return VAR_TYPE_DOUBLE;
				}
				break;
			}
			throw runtime_error("implement all");
		}

		int variant_datatype::val_as_i32() const {
			switch (mType) {

			case VAR_TYPE_STRING:
				//TODO: hex
				return _wtoi(mStrVal.c_str());

			case VAR_TYPE_I32:
				return mI32Val;

			case VAR_TYPE_I64:
				return static_cast<int>(mI64Val);

			case VAR_TYPE_DOUBLE:
				return static_cast<int>(mFVal);
			}

			//TODO: handle all
		}

		int64_t variant_datatype::val_as_i64() const {
			switch (mType) {

			case VAR_TYPE_STRING:
				//TODO: hex
				return _wtoi64(mStrVal.c_str());

			case VAR_TYPE_I32:
				return static_cast<int64_t>(mI32Val);

			case VAR_TYPE_I64:
				return mI64Val;

			case VAR_TYPE_UI64:
				return static_cast<int64_t>(mUI64Val);

			case VAR_TYPE_DOUBLE:
				return static_cast<int64_t>(mFVal);
			}

			//TODO: handle all
		}

		wostream& operator<<(wostream &os, const variant_datatype &var) {
			os << (var.is_empty() ? L"null" : var.val_as_string());
			return os;
		}
	}
}
