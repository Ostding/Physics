mergeInto(LibraryManager.library, {
    js_console_log_int: function (str, param) {
        console.log("call js:", UTF8ToString(str), param, typeof (param));
    },
    js_console_log_int64: function (str, param) {
        console.log("call js:", UTF8ToString(str), param, typeof (param));
    }
});