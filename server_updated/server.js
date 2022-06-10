const express = require('express');
var cors = require('cors');
const cookieParser = require("cookie-parser");
const sessions = require('express-session');
var bodyParser = require('body-parser')
var upnpClient = require('node-upnp-client');
const { spawn } = require('child_process');
const multer = require("multer");

var port = 3000;
const app = express()

const TEST = false

var process = null;



const fs = require('fs');
if (TEST)
    var effectDir = '../raspberry_pedal/out/'
else
    var effectDir = '/home/pi/raspberry_pedal/out/'

var effList = fs.readdirSync(effectDir)


//const upload = multer({ dest: effectDir });

const storage = multer.diskStorage({
    destination: function (req, file, cb) {
        cb(null, effectDir)
    },
    filename: function (req, file, cb) {
        //console.log(file)

        cb(null, file.originalname)
    }
})

const upload = multer({ storage: storage })

if (TEST)
    var pubRoot = 'public';
else
    var pubRoot = '/home/pi/server_updated/public';


const myusername = 'hello'
const mypassword = '1'



app.use(cookieParser());
app.use(cors());

app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())



const oneDay = 1000 * 60 * 60 * 24;
app.use(sessions({
    secret: "sicret",
    saveUninitialized: true,
    cookie: { maxAge: oneDay },
    resave: false
}));




app.get('/admin.html', (req, res) => {
    var session = req.session;
    if (session.userid) {
        res.sendFile('/public/admin.html', { root: __dirname })
    } else
        res.redirect("/login.html")
})


app.post('/login', (req, res) => {
    if (req.body.username == myusername && req.body.password == mypassword) {
        session = req.session;
        session.userid = req.body.username;
        //console.log(req.session)

        res.redirect("/admin.html")


    }else if(req.body.password == '' && req.body.username == '') {
        console.log("Enter username and password");
    }else if(req.body.password == '' ) {
        console.log('Enter password');
    }else if(req.body.password == '' ) {
        console.log('Enter login');
    }else if(req.body.password != mypassword && req.body.username != myusername) {
        console.log('Invalid username and password');
    }
    else if(req.body.username != myusername) {
        console.log('Invalid username');
    }else if(req.body.password != mypassword ) {
        console.log('Invalid password');
    }
})


app.get("/download",(req,res)=>{

 if (req.session.userid) {
	fname = decodeURIComponent(req.query.fname)
        res.sendFile(effectDir+fname, { root: "/" })
    } else {
        res.redirect('/login.html');
    }
})
app.get("/rmf",(req,res)=>{

 if (req.session.userid) {
 fname = decodeURIComponent(req.query.fname)
 fs.unlink(effectDir+fname, function(err){
    if (err) {
        console.log(err);
  res.redirect('/getFileList');
    } else {
        console.log("���� �����");
  effList = fs.readdirSync(effectDir)
  res.redirect('/getFileList');
    }
});
            } else {
        res.redirect('/login.html');
    }
})


app.get('/getFileList', (req, res) => {
    if (req.session.userid) {
        var out = "";
        for (var elem of effList) {
            out += `<a style = "text-decoration: none;
    color: black;">${elem}</a>

    <a download="${elem}" href="/download?fname=${encodeURIComponent( elem)}"style = "color: black;
    position: absolute;
    right: 0;
    margin-right: 30px;"><img src="images/download.png" width="15" height="15"></a>
    
    <a href="/rmf?fname=${encodeURIComponent(elem)} "style = "color: black;
    position: absolute;
    right: 0;
    margin-right: 10px;"><img src="images/delete.png" width="15" height="15"></a><br>`

        }
        res.send(out)
    } else {
        res.redirect('/login.html');
    }
})

app.get('/api/effect', (req, res) => {

    if (effList.indexOf(req.query.eff) >= 0) {
        if (process != null) process.kill('SIGINT');
        process = spawn(effectDir + req.query.eff, [req.query.v1, req.query.v2]);
        res.json({ status: "OK" });
    } else {
        res.json({ error: "effect-" + req.query.eff + " not found" });
    }

});


app.post('/cmd', (req, res) => {
    if (req.session.userid) {
        //console.log(req.body)


        var cmd = decodeURIComponent(req.body.cmd).split(' ')
        if(TEST)
	{}
        else
            ls = spawn(cmd.shift(), cmd);

        var timeout = setTimeout(function () { ls.kill(), res.send('\nTimeout') }, 5000);

        var output = '';
        ls.stdout.on('data', (data) => {
            //console.log(data)
            output += '\n' + data;
        });

        ls.on('close', (code) => {
            //console.log(code)
            clearTimeout(timeout);
            res.send(output)
        });



    } else {
        res.redirect('/login.html');
    }
})


app.post("/upload", [(req, res, next) => {
    if (req.session.userid) {
        next();
    } else {
        res.redirect('/login.html');
    }
}, upload.array("files"), (req, res) => {
    //res.json({ message: "Successfully uploaded files" });
    effList = fs.readdirSync(effectDir)
    res.redirect('/admin.html')

}]);


app.use(express.static(pubRoot));

app.listen(port, () => {
    console.log("run")
})
