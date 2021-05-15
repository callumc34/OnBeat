function display(element) {
    for (let ele of document.body.children) {
        ele.style.display = "none";
    }

    element.style.display = "block";
}


//Event listeners
//HTML onclick= is reserverd for C function callbacks
SettingsQuit.addEventListener("click", () => display(MainMenu))
GameSettings.addEventListener("click", () => display(Settings));