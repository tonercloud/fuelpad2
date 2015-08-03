// Can't use .pragma library (=stateless) since Component.Ready
// does not exist for stateless library

//.pragma library

var component;

function loadComponent(file, parent, properties) {
    component = Qt.createComponent(file)
    if (component.status == Component.Ready) {
        var comp = finishCreation(parent, properties);
        return comp;
    }
    else {
        component.statusChanged.connect(finishCreation);
    }
}

function finishCreation(parent, properties) {
     if (component.status == Component.Ready) {
         var comp = component.createObject(parent, properties);
         if (comp == null) {
             // Error Handling
             console.log("Error creating object");
         }
         return comp;
     } else if (component.status == Component.Error) {
         // Error Handling
         console.log("Error loading component:", component.errorString());
     }
 }

// From http://stackoverflow.com/questions/8224851/how-to-format-stringnumbers-printing-in-javascript
// Usage1: (number).zeroPad()
// Usage2: (number).zeroPad(100)
Number.prototype.zeroPad = Number.prototype.zeroPad ||
    function(base){
            base = base || 10;
            var  len = (String(base).length - String(this).length)+1;
            return len > 0 ? new Array(len).join('0')+this : this;
};

// From http://stackoverflow.com/questions/610406/javascript-equivalent-to-printf-string-format
String.form = function(str, arr) {
    var i = -1;
    function callback(exp, p0, p1, p2, p3, p4) {
        if (exp=='%%') return '%';
        if (arr[++i]===undefined) return undefined;
        var exp  = p2 ? parseInt(p2.substr(1)) : undefined;
        var base = p3 ? parseInt(p3.substr(1)) : undefined;
        var val;
        switch (p4) {
            case 's': val = arr[i]; break;
            case 'c': val = arr[i][0]; break;
            case 'f': val = parseFloat(arr[i]).toFixed(exp); break;
            case 'p': val = parseFloat(arr[i]).toPrecision(exp); break;
            case 'e': val = parseFloat(arr[i]).toExponential(exp); break;
            case 'x': val = parseInt(arr[i]).toString(base?base:16); break;
            case 'd': val = parseFloat(parseInt(arr[i], base?base:10).toPrecision(exp)).toFixed(0); break;
        }
        val = typeof(val)=='object' ? JSON.stringify(val) : val.toString(base);
        var sz = parseInt(p1); /* padding size */
        var ch = p1 && p1[0]=='0' ? '0' : ' '; /* isnull? */
        while (val.length<sz) val = p0 !== undefined ? val+ch : ch+val; /* isminus? */
       return val;
    }
    var regex = /%(-)?(0?[0-9]+)?([.][0-9]+)?([#][0-9]+)?([scfpexd])/g;
    return str.replace(regex, callback);
}

String.prototype.$ = function() {
    return String.form(this, Array.prototype.slice.call(arguments));
}
