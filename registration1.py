class UserEquipment:
    def _init_(self):
        self.state = 'DEREGISTERED'
        self.registration_area = None
        self.amf_info = None
 
    def power_on(self):
        print("UE powered on. Beginning registration process.")
        self.initial_ue_message()
 
    def initial_ue_message(self):
        if self.state == 'DEREGISTERED':
            print("UE -> RAN: Initial UE Message (Registration Request)")
            print("RAN -> AMF: Forwarding UE's Registration Request")
            self.receive_registration_accept()
 
    def receive_registration_accept(self):
        print("AMF -> RAN: Registration Accept")
        print("RAN -> UE: Forwarding Registration Accept")
        self.state = 'REGISTERED'
        self.update_registration_area('Area A')
        self.configuration_update_command()
 
    def update_registration_area(self, area):
        self.registration_area = area
        print(f"Registration area updated to {self.registration_area}.")
 
    def configuration_update_command(self):
        print("AMF -> RAN: Configuration Update Command")
        print("RAN -> UE: Forwarding Configuration Update Command")
        self.send_configuration_update_complete()
 
    def send_configuration_update_complete(self):
        print("UE -> RAN: Configuration Update Complete")
        print("RAN -> AMF: Forwarding Configuration Update Complete")
        self.send_registration_complete()
