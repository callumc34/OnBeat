//C callbacks to JS
//Required JS Functions
function SetSettings(settings) {
    for (let key in settings) {
        let setting = document.getElementById(key);
        if (!setting) {
            continue;
        }
        setting.value = settings[key];                
    }
    COLUMN_1.value = settings["Input"]["COLUMN_1"];
    COLUMN_2.value = settings["Input"]["COLUMN_2"];
    COLUMN_3.value = settings["Input"]["COLUMN_3"];
    COLUMN_4.value = settings["Input"]["COLUMN_4"];
    PAUSE.value = settings["Input"]["PAUSE"];
}

function DOMLoaded() {
    setHWInfo();
    RevertSettings();
}

//JS functions - non required
function setHWInfo() {
    info = GetHWInfo();
    //Set monitors
    for (let m = 1; m <= info.monitors; m++) {
        let opt = document.createElement("option");
        opt.value = m;
        opt.appendChild(document.createTextNode("Monitor " + m));

        Fullscreen.appendChild(opt);
    }

    //Set resolution values
    var resolutions = [
        {width: 7680, height: 4320, res: "7680x4320"},
        {width: 3840, height: 2160, res: "3840x2160"},
        {width: 3840, height: 1440, res: "3840x1440"},
        {width: 2560, height: 1440, res: "2560x1440"},
        {width: 1920, height: 1080, res: "1920x1080"},
        {width: 1920, height: 1200, res: "1920x1200"},
        {width: 1680, height: 1050, res: "1680x1050"},
        {width: 1600, height: 900, res: "1600x900"},
        {width: 1440, height: 900, res: "1440x900"},
        {width: 1366, height: 768, res: "1366x768"},
        {width: 1280, height: 720, res: "1280x720"},
        {width: 1024, height: 768, res: "1024x768"},
    ];
    
    for (let res of resolutions) {
        if (res.width > info.width || res.height > info.height) {
            continue;
        }

        let opt = document.createElement("option");
        opt.value = res.res;
        opt.appendChild(document.createTextNode(res.res));

        Resolution.appendChild(opt);
    }
}

function getAsNumber(val) {
    let value = Number(val);
    if (value == NaN) {
        return val;
    }
    return value;
}

function applySettings() {
    let config = {
        "Resolution": Resolution.value,
        "Fullscreen": Number(Fullscreen.value),
        "VSync": Number(VSync.value),
        "FpsCap": FpsCap.valueAsNumber,
        "Skin": Skin.value,
        "Volume": Volume.valueAsNumber,

        "Input": {
            "COLUMN_1" : COLUMN_1.value,
            "COLUMN_2" : COLUMN_2.value,
            "COLUMN_3" : COLUMN_3.value,
            "COLUMN_4" : COLUMN_4.value,
            "PAUSE"       : PAUSE.value
        }
    };
    UpdateSettings(config);
}   

function display(element, hide, style) {
    for (let ele of hide) {
        ele.style.display = "none";
    }

    element.style.display = style;
}

function setSkin() {
    let path = SelectSkin();
    if (path) {
        Skin.value = path;
    }
}

function codeToString(key) {
    switch (key) {
        case 8:
            return "Backspace";
        case 9:
            return "Tab";
        case 13:
            return "Enter";
        case 27:
            return "Escape";
        case 32:
            return "Space";
        case 39:
            return "Apoostrophe";
        case 44:
            return "Comma";
        case 45:
            return "Minus";
        case 46:
            return "Period";
        case 47:
            return "Slash";
        case 48:
            return "D0";
        case 49:
            return "D1";
        case 50:
            return "D2";
        case 51:
            return "D3";
        case 52:
            return "D4";
        case 53:
            return "D5";
        case 54:
            return "D6";
        case 55:
            return "D7";
        case 56:
            return "D8";
        case 57:
            return "D9";
        default:
            let char = String.fromCharCode(key.toUpperCase());
            if (char == "") {
                return "Undefined";
            }
            return char;
    }
}

function keyUpCheck(event) {  
    this.value = codeToString(event.keyCode);
}

function inputToUpper(event) {
    this.value = event.data.toUpperCase();
}


//Event listeners
GameSettings.addEventListener("click", () => display(Settings,
 document.body.children, "block"));

DisplayHeader.addEventListener("click", () => display(
    DisplaySettings, document.getElementsByClassName("settingsOptions"), "flex"));

InputHeader.addEventListener("click", () => display(
    InputSettings, document.getElementsByClassName("settingsOptions"), "flex"));

AudioHeader.addEventListener("click", () => display(
    AudioSettings, document.getElementsByClassName("settingsOptions"), "flex"));


SettingsQuit.addEventListener("click", () => display(MainMenu,
 document.body.children, "block"));

VolumeSelect.onchange = function() {
    Volume.value = this.value;
}

for (let ele of document.getElementsByClassName("KeyInput")) {
    ele.addEventListener("input", inputToUpper);
    ele.addEventListener("keyup", keyUpCheck);
}


Volume.onchange = function() {
    if (isNaN(this.value) || Number(this.value) > this.max || Number(this.value) < this.min) {
        this.value = VolumeSelect.value;
        return;
    }
    VolumeSelect.value = this.value;
}

//Initial setup calls
DisplayHeader.click();
