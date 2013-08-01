// Application with the Sheet project template
import bb.cascades 1.0

Page {
    id: root
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
            text: "sash>"
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0
            }
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
        }

    }
    actions: [
        ActionItem {
            title: "Restart"
            onTriggered: {
                runButton.enabled = false;
                repl.restart();
                display.text = "sash>";
            }
        }
    ]
    // from repl
    function replReady() {
        runButton.enabled = true;
    }
    function replError() {
        runButton.enabled = false;
        display.text = "Abnormal socket termination, please restart REPL."
    }
}
