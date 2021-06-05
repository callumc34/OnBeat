//C callbacks to JS
//Required JS Functions
function setSettings(settings) {
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

//JS functions - non required
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
        "Fullscreen": Fullscreen.valueAsNumber,
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