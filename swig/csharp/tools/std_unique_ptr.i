
%include <typemaps.i>
%include <attribute.i>

%define %unique_ptr(TYPE)
    %typemap (ctype) std::auto_ptr<TYPE > "void *"
    %typemap (imtype, out="global::System.IntPtr") std::unique_ptr<TYPE > "HandleRef"
    %typemap (cstype) std::unique_ptr<TYPE > "$typemap(cstype, TYPE)"
    %typemap (out) std::unique_ptr<TYPE > %{
        $result = (void *)$1.release();
    %}
    %typemap(csout, excode=SWIGEXCODE) std::unique_ptr<TYPE > {
        global::System.IntPtr cPtr = $imcall;
        $typemap(cstype, TYPE) ret = (cPtr == global::System.IntPtr.Zero) ? null : new $typemap(cstype, TYPE)(cPtr, true);$excode
        return ret;
    }

    // Out var handling
    %typemap (ctype) std::auto_ptr<TYPE > & OUTPUT "void **"
    %typemap (imtype, out="global::System.IntPtr") std::unique_ptr<TYPE > & OUTPUT "out global::System.IntPtr"
    %typemap (cstype) std::unique_ptr<TYPE > & OUTPUT "out $typemap(cstype, TYPE)"

    %typemap(csin,
        pre="    global::System.IntPtr cPtr_$csinput = global::System.IntPtr.Zero;",
        post="      $csinput = (cPtr_$csinput == global::System.IntPtr.Zero) ? null : new $typemap(cstype, TYPE)(cPtr, true);",
        cshin="out $csinput") std::unique_ptr<TYPE > & OUTPUT "out cPtr_$csinput"

    %typemap(in) std::unique_ptr<TYPE > & OUTPUT %{ $1 = new std::unique_ptr<TYPE >((TYPE *)$input); %}
    %typemap(freearg) std::unique_ptr<TYPE > & OUTPUT ""

    // Properties
    %typemap(csvarout, excode=SWIGEXCODE) std::unique_ptr<TYPE > {
        // csvarout std::unique_ptr<TYPE >
        get {
            global::System.IntPtr cPtr = $imcall;
            $typemap(cstype, TYPE) ret = (cPtr == global::System.IntPtr.Zero) ? null : new $typemap(cstype, TYPE)(cPtr, true);$excode
            return ret;
        }
    }

    %template() std::unique_ptr<TYPE >;
%enddef

%define %attribute_unique_ptr(Class, AttributeType, AttributeName, GetMethod, SetMethod...)
    #if #SetMethod != ""
        %attribute_custom(%arg(Class), %arg(AttributeType), AttributeName, GetMethod, SetMethod, self_->GetMethod().release(), self_->SetMethod(val_))
    #else
        %attribute_readonly(%arg(Class), %arg(AttributeType), AttributeName, GetMethod, self_->GetMethod().release())
    #endif
%enddef

namespace std {
    template <class T> class std::unique_ptr {};
}