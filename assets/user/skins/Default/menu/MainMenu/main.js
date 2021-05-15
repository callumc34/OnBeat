function display(element, hide, style) {
    for (let ele of hide) {
        ele.style.display = "none";
    }

    element.style.display = style;
}


//Event listeners
//HTML onclick= is reserverd for C function callbacks
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

//Initial listener calls
DisplayHeader.click();