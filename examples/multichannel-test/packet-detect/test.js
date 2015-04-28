TIMEOUT(600000);

timeout_function = function () {
    log.log("Script timed out.\n");
    log.testOK();
}

while (true) {
    YIELD();
}
