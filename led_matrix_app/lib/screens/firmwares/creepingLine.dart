import 'package:flutter/material.dart';

import 'dart:typed_data';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:web_socket_channel/io.dart';
import '../../proto/clock/clock.pb.dart';

List<int> utf8ToCP1251(String sourceString) {
  var convertedBytes = List<int>();
  sourceString.runes.forEach((int rune) {
    if (rune <= 126) {
      convertedBytes.add(rune);
    } else if (1040 <= rune && rune <= 1103) {
      convertedBytes.add(192 + rune - 1040);
    }
  });
  return convertedBytes;
}

class CreepingLine extends StatefulWidget {
  @override
  _CreepingLineState createState() => _CreepingLineState();
}

class _CreepingLineState extends State<CreepingLine> {
  String _ip;

  setText(String text) {
    var uri = 'ws://$_ip/clock/ws';
    var channel = IOWebSocketChannel.connect(uri);

    var message = ClockMessage();
    message.modeId = 1;

    var cp1251Copepoints = utf8ToCP1251(text);
    message.text = cp1251Copepoints +
        new List<int>.filled(255 - cp1251Copepoints.length, 0, growable: false);

    print(text);
    channel.sink.add(message.writeToBuffer());
    channel.sink.close();
  }

  @override
  void initState() {
    super.initState();
    _loadSettings();
  }

  _loadSettings() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    _ip = (prefs.getString('ip') ?? "");
  }

  final _formKey = GlobalKey<FormState>();
  String _lineText;

  @override
  Widget build(BuildContext context) {
    return Container(
      child: Form(
        key: _formKey,
        // height: double.infinity,
        child: Column(
          children: <Widget>[
            TextFormField(
              onChanged: (value) {
                _lineText = value;
              },
              validator: (value) {
                if (value.isEmpty) {
                  return 'Введіть текст!';
                }
                return null;
              },
            ),
            RaisedButton(
              child: Text("Встановити текст"),
              onPressed: () {
                if (_formKey.currentState.validate()) {
                  setText(_lineText);
                }
              },
            )
          ],
        ),
      ),
    );
  }
}
