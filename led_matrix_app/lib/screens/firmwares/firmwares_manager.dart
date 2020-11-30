import 'package:flutter/material.dart';

import 'animations.dart';
import 'games.dart';
import 'canvas.dart';
import 'stopwatch.dart';
import 'clock.dart';
import 'creepingLine.dart';

class FirmwaresManager extends StatelessWidget {
  final String mode;

  const FirmwaresManager({
    Key key,
    this.mode
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    switch (mode){
      case "clock":
        return Clock();
      case "stopwatch":
        return Stopwatch();
      case "creepingLine":
        return CreepingLine();
      case "animations":
        return AnimationSelectStatefulWidget();
      case "games":
        return GameSelectionStatefulWidget();
      case "canvas":
        return Text("Canvas");

    }
    return Container(

    );
  }
}

