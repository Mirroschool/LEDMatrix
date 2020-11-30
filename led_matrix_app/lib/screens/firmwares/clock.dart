import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'package:shared_preferences/shared_preferences.dart';

class Clock extends StatefulWidget {
  @override
  _ClockState createState() => _ClockState();
}

class _ClockState extends State<Clock> {
  String _ip;

  Future<http.Response> syncTime() async {
    var uri = Uri.parse('http://$_ip/modes/');
    var request = http.MultipartRequest('POST', uri)
      ..fields['mode_id'] = "3"
      ..fields['time'] =
          (DateTime.now().millisecondsSinceEpoch ~/ 1000 + 7200).toString();

    var streamedResponse = await request.send();
    var response = await http.Response.fromStream(streamedResponse);
    return response;
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

  @override
  Widget build(BuildContext context) {
    print("CALLEED");
    return Container(
      child: SizedBox(
        width: double.infinity,
        // height: double.infinity,
        child: RaisedButton(
          child: Text("Синхронізувати час"),
          onPressed: () async {
            await syncTime();
          },
        ),
      ),
    );
  }
}
