

function GET(theUrl, callback) {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function () {
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
            callback(xmlHttp.responseText);
    }
    xmlHttp.open("GET", theUrl, true); // true for asynchronous 
    xmlHttp.send(null);
}

function setEffect(eff,val1,val2){
    GET(`/api/effect?eff=${eff}&v1=${val1}&v2=${val2}`,(t)=>{console.log(t)})
}

function setOverdrive() {
    setEffect('overdrive',document.getElementById('Overdrive1').value,document.getElementById('Overdrive2').value)
}

function setOctaver() {
    setEffect('octaver',document.getElementById('Octaver1').value,document.getElementById('Octaver2').value)
}

function setDelay() {
    setEffect('delay',document.getElementById('Delay1').value,document.getElementById('Delay2').value)
}

function setReverb() {
    setEffect('reverb',document.getElementById('Reverb1').value,document.getElementById('Reverb2').value)
}

function setEcho() {
    setEffect('echo',document.getElementById('Echo1').value,document.getElementById('Echo2').value)
}

function setFuzz() {
    setEffect('fuzz',document.getElementById('Fuzz1').value,document.getElementById('Fuzz2').value)
}



