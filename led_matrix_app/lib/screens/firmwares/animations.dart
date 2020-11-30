import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'package:shared_preferences/shared_preferences.dart';

enum AnimationType { fire, water, fade, rainbow, cylon }


/// This is the stateful widget that the main application instantiates.
class AnimationSelectStatefulWidget extends StatefulWidget {
  AnimationSelectStatefulWidget({Key key}) : super(key: key);

  @override
  _AnimationSelectStatefulWidgetState createState() =>
      _AnimationSelectStatefulWidgetState();
}

/// This is the private State class that goes with MyStatefulWidget.
class _AnimationSelectStatefulWidgetState
    extends State<AnimationSelectStatefulWidget> {

  String _ip;
  AnimationType _character = AnimationType.fire;
  double _currentFPSSliderValue = 15;
  double _currentBrightnessSliderValue = 20;

  Future<http.Response> setAnimationId(int animationId) async {
    var uri = Uri.parse('http://$_ip/animations/');
    var request = http.MultipartRequest('POST', uri)
      ..fields['animation_id'] = animationId.toString();

    var streamedResponse = await request.send();
    var response = await http.Response.fromStream(streamedResponse);
    return response;
  }

  Future<http.Response> setFPS(int fps) async {
    var uri = Uri.parse('http://$_ip/fps/');
    var request = http.MultipartRequest('POST', uri)
      ..fields['fps'] = fps.toString();

    var streamedResponse = await request.send();
    var response = await http.Response.fromStream(streamedResponse);
    return response;
  }

  Future<http.Response> setBrightness(int brightness) async {
    var uri = Uri.parse('http://$_ip/brightness/');
    var request = http.MultipartRequest('POST', uri)
      ..fields['brightness'] = brightness.toString();

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
    return Column(
      children: <Widget>[
        RadioListTile<AnimationType>(
          title: const Text('Вогонь'),
          value: AnimationType.fire,
          groupValue: _character,
          onChanged: (AnimationType value) async {
            var response = await setAnimationId(1);
            if (response.statusCode == 200) {
              setState(() {
                _character = value;
              });
            }
          },
        ),
        RadioListTile<AnimationType>(
          title: const Text('Океан'),
          value: AnimationType.water,
          groupValue: _character,
          onChanged: (AnimationType value) async {
            var response = await setAnimationId(2);
            if (response.statusCode == 200) {
              setState(() {
                _character = value;
              });
            }
          },
        ),
        RadioListTile<AnimationType>(
          title: const Text('Затухання'),
          value: AnimationType.fade,
          groupValue: _character,
          onChanged: (AnimationType value) async {
            var response = await setAnimationId(3);
            if (response.statusCode == 200) {
              setState(() {
                _character = value;
              });
            }
          },
        ),
        RadioListTile<AnimationType>(
          title: const Text('Веселка'),
          value: AnimationType.rainbow,
          groupValue: _character,
          onChanged: (AnimationType value) async {
            var response = await setAnimationId(4);
            if (response.statusCode == 200) {
              setState(() {
                _character = value;
              });
            }
          },
        ),
        RadioListTile<AnimationType>(
          title: const Text('Стрічка'),
          value: AnimationType.cylon,
          groupValue: _character,
          onChanged: (AnimationType value) async {
            var response = await setAnimationId(5);
            if (response.statusCode == 200) {
              setState(() {
                _character = value;
              });
            }
          },
        ),
        const Divider(
          color: Colors.grey,
          height: 20,
          thickness: 1,
          indent: 0,
          endIndent: 0,
        ),
        Text(
          "FPS",
          style: TextStyle(fontSize: 20),
        ),
        Slider(
          value: _currentFPSSliderValue,
          min: 1,
          max: 100,
          divisions: 100,
          label: _currentFPSSliderValue.round().toString(),
          onChanged: (double value) {
            setState(() {
              _currentFPSSliderValue = value;
            });
          },
          onChangeEnd: (double value) async {
            await setFPS(_currentFPSSliderValue.toInt());
          },
        ),
        Text(
          "Яскравість",
          style: TextStyle(fontSize: 20),
        ),
        Slider(
          value: _currentBrightnessSliderValue,
          min: 0,
          max: 255,
          divisions: 255,
          label: _currentBrightnessSliderValue.round().toString(),
          onChanged: (double value) {
            setState(() {
              _currentBrightnessSliderValue = value;
            });
          },
          onChangeEnd: (double value) async {
            await setBrightness(_currentBrightnessSliderValue.toInt());
          },
        ),
      ],
    );
  }
}