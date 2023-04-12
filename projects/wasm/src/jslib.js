mergeInto(LibraryManager.library, {
    js_console_log_int: function (param) {
        console.log(param);
        console.log(typeof (param));
    }
});