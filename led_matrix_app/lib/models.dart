import 'dart:async';
import 'dart:convert';
import 'dart:io';

import 'package:http/http.dart' as http;

class Settings {
  final List<String> modes;
  final int width;
  final int height;

  Settings({this.modes, this.width, this.height});

  factory Settings.fromJson(Map<String, dynamic> json) {
    return Settings(
      modes: json['modes'].cast<String>(),
      width: int.parse(json['width']),
      height: int.parse(json['height']),
    );
  }

  static Future<Settings> getSettings(String ip) async {
    var uri = Uri.parse('http://$ip/settings/');

    try {
      final response = await http.get(uri).timeout(new Duration(seconds: 3));
      if (response.statusCode == 200) {
        return Settings.fromJson(jsonDecode(response.body));
      } else {
        throw Exception("Bad status code!");
      }
    } on TimeoutException catch (_) {
      throw _;
    } on SocketException catch (_) {
      throw _;
    }
  }
}
