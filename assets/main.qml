// Application with the Sheet project template
import bb.cascades 1.0

Page {
    Container {
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            TextArea {
                id: expr
                hintText: "Type expression"
            }
            Button {
                id: runButton
                text: qsTr("Run")
                enabled: false; // this will be enabled in control
                onClicked: {
                    runButton.enabled = false;
                    display.text += expr.text + "\n" + repl.execute(expr.text) + "sash>";
                    expr.resetText();
                    runButton.enabled = true;
                }
                opacity: 1.0
            }
        }

        TextArea {
            id: display
            hintText: ""
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0
            }
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            onTextChanging: {
                if (display.text == "") display.text = "sash>"
            }
        }

    }
    actions: [
        ActionItem {
            title: "Restart"
            onTriggered: {
                runButton.enabled = false;
                repl.restart();
            }
        },
        ActionItem {
            title: "Clear"
            onTriggered: {
                display.text = "";
            }
        }
    ]
    // from repl
    function replReady() {
        runButton.enabled = true;
        display.text = "sash>";
    }
    function replError() {
        runButton.enabled = false;
        display.text = "Abnormal socket termination, please restart REPL."
    }
}
