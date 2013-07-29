// Application with the Sheet project template
import bb.cascades 1.0

Page {
    // Sheet page containing example content which will be probabbly changed in real application.
    property alias title: edTitle.text
    property alias comment: edComment.text
    property bool isAppend
    signal done(bool ok)
    function addComment() {
        edTitle.resetText();
        edComment.resetText();
        isAppend = true;
    }
    function editComment(data) {
        if(data) {
            title = data.title;
            comment = data.comment;
        }
        isAppend = false;
    }
    Container {
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Button {
                text: qsTr("Cancel")
                onClicked: {
                    done(false)
                }
            }
            Label {
                text: qsTr("Comment details")
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                }
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
            }
            Button {
                text: qsTr("Ok")
                onClicked: {
                    done(true)
                }
                opacity: 1.0
            }
        }
        Label {
            text: qsTr("Title:")
        }
        TextField {
            id: edTitle
        }
        Label {
            text: qsTr("Comment:")
        }
        TextArea {
            id: edComment
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0
            }
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
        }
    }
}
