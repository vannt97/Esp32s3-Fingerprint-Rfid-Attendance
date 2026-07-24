from rest_framework import serializers

from .models import AttendanceRecord, Employee


class EmployeeSerializer(serializers.ModelSerializer):
    class Meta:
        model = Employee
        fields = ["id", "name"]


class AttendanceRecordSerializer(serializers.ModelSerializer):
    class Meta:
        model = AttendanceRecord
        fields = ["id", "employee", "method", "event_time", "client_record_id"]
        read_only_fields = ["id"]
