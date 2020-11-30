import 'package:flutter/material.dart';

import 'screens/landing.dart';
import 'screens/login.dart';
import 'screens/home.dart';


void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Login Demo',
      routes: {
        '/': (context) => Landing(),
        '/login': (context) => Login(),
        '/home': (context) => MyHomePage(title: 'Login Demo'),
      },
      theme: ThemeData(
        primarySwatch: Colors.green,
      ),
    );
  }
}
