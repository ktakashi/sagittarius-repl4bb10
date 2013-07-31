// Application with the Sheet project template
import bb.cascades 1.0

Page {
    Container {
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            TextField {
                id: expr
                hintText: "Type expression"
            }
            Button {
                text: qsTr("Run")
                onClicked: {
                    display.text().append(repl.execute(expt.text));
                    expr.resetText();
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
        }
    }
}
