from django.utils import timezone
from rest_framework import generics, status
from rest_framework.response import Response

from .models import AttendanceRecord, Employee
from .serializers import AttendanceRecordSerializer, EmployeeSerializer


def _touch_device(device):
    device.last_seen_at = timezone.now()
    device.save(update_fields=["last_seen_at"])


class EmployeeListView(generics.ListAPIView):
    serializer_class = EmployeeSerializer

    def get_queryset(self):
        _touch_device(self.request.user.device)
        return Employee.objects.filter(is_active=True).order_by("name")


class AttendanceCreateView(generics.CreateAPIView):
    serializer_class = AttendanceRecordSerializer

    def create(self, request, *args, **kwargs):
        device = request.user.device
        _touch_device(device)

        # Idempotent: nếu thiết bị gửi lại đúng client_record_id đã ghi
        # trước đó (do retry sau mất kết nối), trả bản ghi cũ thay vì tạo
        # bản ghi trùng.
        client_record_id = request.data.get("client_record_id")
        if client_record_id:
            existing = AttendanceRecord.objects.filter(
                device=device, client_record_id=client_record_id
            ).first()
            if existing:
                return Response(self.get_serializer(existing).data, status=status.HTTP_200_OK)

        serializer = self.get_serializer(data=request.data)
        serializer.is_valid(raise_exception=True)
        serializer.save(device=device)
        return Response(serializer.data, status=status.HTTP_201_CREATED)
