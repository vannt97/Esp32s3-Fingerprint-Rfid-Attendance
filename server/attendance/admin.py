from django.contrib import admin

from .models import AttendanceRecord, Device, Employee


@admin.register(Employee)
class EmployeeAdmin(admin.ModelAdmin):
    list_display = ["id", "name", "is_active", "created_at"]
    list_filter = ["is_active"]
    search_fields = ["name"]


@admin.register(Device)
class DeviceAdmin(admin.ModelAdmin):
    list_display = ["id", "name", "user", "last_seen_at"]
    search_fields = ["name"]


@admin.register(AttendanceRecord)
class AttendanceRecordAdmin(admin.ModelAdmin):
    list_display = ["id", "employee", "device", "method", "event_time", "received_at"]
    list_filter = ["method", "device", "event_time"]
    search_fields = ["employee__name"]
