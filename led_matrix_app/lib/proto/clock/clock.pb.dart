///
//  Generated code. Do not modify.
//  source: clock.proto
//
// @dart = 2.3
// ignore_for_file: annotate_overrides,camel_case_types,unnecessary_const,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type,unnecessary_this,prefer_final_fields

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

class ClockMessage extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'ClockMessage', createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'modeId', $pb.PbFieldType.O3)
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'timestamp', $pb.PbFieldType.O3)
    ..a<$core.List<$core.int>>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'text', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  ClockMessage._() : super();
  factory ClockMessage() => create();
  factory ClockMessage.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ClockMessage.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ClockMessage clone() => ClockMessage()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ClockMessage copyWith(void Function(ClockMessage) updates) => super.copyWith((message) => updates(message as ClockMessage)); // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static ClockMessage create() => ClockMessage._();
  ClockMessage createEmptyInstance() => create();
  static $pb.PbList<ClockMessage> createRepeated() => $pb.PbList<ClockMessage>();
  @$core.pragma('dart2js:noInline')
  static ClockMessage getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ClockMessage>(create);
  static ClockMessage _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get modeId => $_getIZ(0);
  @$pb.TagNumber(1)
  set modeId($core.int v) { $_setSignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasModeId() => $_has(0);
  @$pb.TagNumber(1)
  void clearModeId() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get timestamp => $_getIZ(1);
  @$pb.TagNumber(2)
  set timestamp($core.int v) { $_setSignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTimestamp() => $_has(1);
  @$pb.TagNumber(2)
  void clearTimestamp() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<$core.int> get text => $_getN(2);
  @$pb.TagNumber(3)
  set text($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasText() => $_has(2);
  @$pb.TagNumber(3)
  void clearText() => clearField(3);
}

