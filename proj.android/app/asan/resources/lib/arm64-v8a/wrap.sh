#!/system/bin/sh
# Loads the AddressSanitizer runtime before the app starts (ASan builds only).
# The mismatch checks are off because system codecs (libsfplugin_ccodec) trip them on their own.
# Reports go to the tombstone abort message, to files/asan.<pid>, and (via stderr) to files/asan-stderr.log;
# files/wrap.log records each launch. Read them with `adb shell run-as com.vauzi.narutosenki cat files/<name>`.
HERE="$(cd "$(dirname "$0")" && pwd)"
LOGDIR=/data/data/com.vauzi.narutosenki/files
mkdir -p "$LOGDIR"
export ASAN_OPTIONS=log_to_syslog=true,abort_on_error=1,detect_leaks=0,malloc_context_size=30,new_delete_type_mismatch=0,alloc_dealloc_mismatch=0,log_path=$LOGDIR/asan
ASAN_LIB=$(ls "$HERE"/libclang_rt.asan-*-android.so)
if [ -f "$HERE/libc++_shared.so" ]; then
    export LD_PRELOAD="$ASAN_LIB $HERE/libc++_shared.so"
else
    export LD_PRELOAD="$ASAN_LIB"
fi
echo "$(date '+%F %T') launch pid=$$ asan=$ASAN_LIB" >> "$LOGDIR/wrap.log"
exec "$@" 2>> "$LOGDIR/asan-stderr.log"
