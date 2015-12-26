import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    property string titleText
    property string message
    property string acceptButtonText: "Accept"
    property string rejectButtonText: "Reject"

    signal accepted

    // Todo: impelement titleText, message, accept button, reject button
}
