#!/system/bin/sh
# Loads the AddressSanitizer runtime before the app starts (ASan builds only).
# Reports go to the tombstone abort message and to /data/data/com.vauzi.narutosenki/asan.<pid>.
# The mismatch checks are off because system codecs (libsfplugin_ccodec) trip them on their own.
HERE="$(cd "$(dirname "$0")" && pwd)"
export ASAN_OPTIONS=log_to_syslog=false,abort_on_error=1,detect_leaks=0,malloc_context_size=30,new_delete_type_mismatch=0,alloc_dealloc_mismatch=0,log_path=/data/data/com.vauzi.narutosenki/asan
ASAN_LIB=$(ls "$HERE"/libclang_rt.asan-*-android.so)
if [ -f "$HERE/libc++_shared.so" ]; then
    export LD_PRELOAD="$ASAN_LIB $HERE/libc++_shared.so"
else
    export LD_PRELOAD="$ASAN_LIB"
fi
"$@"
