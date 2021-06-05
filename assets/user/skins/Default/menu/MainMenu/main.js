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

function setKeyCode(event) {
    document.getElementById(this.dataset.setting).value = event.keyCode;
}

function preventKeyAppear(event) {
    this.value = "";
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
    ele.addEventListener("keyup", setKeyCode);
    ele.addEventListener("keydown", preventKeyAppear);
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