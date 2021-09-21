%define %unique_ptr(TYPE)
    %typemap (ctype) std::auto_ptr<TYPE > "void *"
    %typemap (imtype, out="System.IntPtr") std::unique_ptr<TYPE > "HandleRef"
    %typemap (cstype) std::unique_ptr<TYPE > "$typemap(cstype, TYPE)"
    %typemap (out) std::unique_ptr<TYPE > %{
        $result = (void *)$1.release();
    %}
    %typemap(csout, excode=SWIGEXCODE) std::unique_ptr<TYPE > {
        System.IntPtr cPtr = $imcall;
        $typemap(cstype, TYPE) ret = (cPtr == System.IntPtr.Zero) ? null : new $typemap(cstype, TYPE)(cPtr, true);$excode
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

    %template() std::unique_ptr<TYPE >;
%enddef

namespace std {
    template <class T> class unique_ptr {};
}