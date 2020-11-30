import 'package:flutter/material.dart';
import 'package:web_socket_channel/io.dart';

import 'package:shared_preferences/shared_preferences.dart';

class GameSelectionStatefulWidget extends StatefulWidget {
  @override
  _GameSelectionStatefulWidgetState createState() =>
      _GameSelectionStatefulWidgetState();
}

class _GameSelectionStatefulWidgetState
    extends State<GameSelectionStatefulWidget> {

  IOWebSocketChannel _channel;
  @override
  void initState() {
    super.initState();
    _loadSettings();
  }

  _loadSettings() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    _channel = IOWebSocketChannel.connect('ws://${(prefs.getString('ip') ?? "")}/games/ws');
  }


  @override
  Widget build(BuildContext context) {
    return Column(mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.center,children: <Widget>[
      MaterialButton(
          child: Icon(Icons.arrow_upward),
          onPressed: () {
            _channel.sink.add("u");
          }),
      MaterialButton(
          child: Icon(Icons.arrow_downward),
          onPressed: () {
            _channel.sink.add("d");
          }),
    ]);
  }
}
