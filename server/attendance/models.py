from django.contrib.auth.models import User
from django.db import models


class Employee(models.Model):
    name = models.CharField(max_length=100)
    is_active = models.BooleanField(default=True)
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return self.name


class Device(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE, related_name="device")
    name = models.CharField(max_length=100)
    last_seen_at = models.DateTimeField(null=True, blank=True)

    def __str__(self):
        return self.name


class AttendanceRecord(models.Model):
    METHOD_CHOICES = [
        ("F", "Fingerprint"),
        ("C", "Card"),
    ]

    employee = models.ForeignKey(Employee, on_delete=models.CASCADE, related_name="attendance_records")
    device = models.ForeignKey(Device, on_delete=models.CASCADE, related_name="attendance_records")
    method = models.CharField(max_length=1, choices=METHOD_CHOICES)
    event_time = models.DateTimeField()
    client_record_id = models.CharField(max_length=64, null=True, blank=True)
    received_at = models.DateTimeField(auto_now_add=True)

    class Meta:
        # client_record_id NULL không bị coi là trùng (chuẩn SQL) nên các
        # bản ghi hiện tại (chưa gửi client_record_id) vẫn ghi bình thường;
        # khi ApiService sau này gửi kèm ID thì mới chống được ghi trùng.
        unique_together = [("device", "client_record_id")]
        ordering = ["-event_time"]

    def __str__(self):
        return f"{self.employee} @ {self.event_time} ({self.method})"
