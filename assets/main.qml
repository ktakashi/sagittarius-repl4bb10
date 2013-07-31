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
                text: qsTr("Run")
                onClicked: {
                    display.text += expr.text + "\n" + repl.execute(expr.text) + "sash>";
                    expr.resetText();
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
}
