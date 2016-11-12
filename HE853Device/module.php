<?

class HE853Device extends IPSModule
{

    public function Create() {
        parent::Create();
        $this->RegisterVariableBoolean("STATE", "STATE", "~Switch");
        $this->EnableAction("STATE");
        $this->RegisterPropertyBoolean("Bit1", false);
        $this->RegisterPropertyBoolean("Bit2", false);
        $this->RegisterPropertyBoolean("Bit3", false);
        $this->RegisterPropertyBoolean("Bit4", false);
        $this->RegisterPropertyBoolean("Bit5", false);
        $this->RegisterPropertyBoolean("BitA", false);
        $this->RegisterPropertyBoolean("BitB", false);
        $this->RegisterPropertyBoolean("BitC", false);
        $this->RegisterPropertyBoolean("BitD", false);
        $this->RegisterPropertyBoolean("BitE", false);
        $this->RegisterPropertyInteger("Repeat", 2);
    }

    public function ApplyChanges() {
        parent::ApplyChanges();
    }

    protected function GetDeviceNumber() {
        if (!$this->ReadPropertyBoolean("Bit1")) {
            $DeviceNumber = '0';
        } else {
            $DeviceNumber = '1';
        }
        if (!$this->ReadPropertyBoolean("Bit2")) {
            $DeviceNumber .= '0';
        } else {
            $DeviceNumber .= '1';
        }
        if (!$this->ReadPropertyBoolean("Bit3")) {
            $DeviceNumber .= '0';
        } else {
            $DeviceNumber .= '1';
        }
        if (!$this->ReadPropertyBoolean("Bit4")) {
            $DeviceNumber .= '0';
        } else {
            $DeviceNumber .= '1';
        }
        if (!$this->ReadPropertyBoolean("Bit5")) {
            $DeviceNumber .= '0';
        } else {
            $DeviceNumber .= '1';
        }
        if (!$this->ReadPropertyBoolean("BitA")) {
            $DeviceNumber .= '0';
        } else {
            $DeviceNumber .= '1';
        }
        if (!$this->ReadPropertyBoolean("BitB")) {
            $DeviceNumber .= '0';
        } else {
            $DeviceNumber .= '1';
        }
        if (!$this->ReadPropertyBoolean("BitC")) {
            $DeviceNumber .= '0';
        } else {
            $DeviceNumber .= '1';
        }
        if (!$this->ReadPropertyBoolean("BitD")) {
            $DeviceNumber .= '0';
        } else {
            $DeviceNumber .= '1';
        }
        if (!$this->ReadPropertyBoolean("BitE")) {
            $DeviceNumber .= '0';
        } else {
            $DeviceNumber .= '1';
        }
        return $DeviceNumber;
    }

    public function RequestAction($Ident, $Value) {
        if ($Ident == 'STATE') {
            $this->DoSwitch($Value);
        }
    }

    public function DoSwitch(boolean $State) {
        $DevNum = $this->GetDeviceNumber();
        $Repeat = $this->ReadPropertyInteger("Repeat");
        if ((bool) $State) {
            $out = exec(__DIR__ . "/../HE853WinLib/he853_switcher.exe " . $DevNum . " 1 " . $Repeat, $output, $result);
        } else {
            $out = exec(__DIR__ . "/../HE853WinLib/he853_switcher.exe " . $DevNum . " 0 " . $Repeat, $output, $result);
        }

        if ($result === 0) {
            SetValueBoolean($this->GetIDForIdent('STATE'), $State);
            return true;
        } else {
            trigger_error('Fehler: ' . $out . ' Result = ' . $result, E_USER_NOTICE);
            return false;
        }
    }

}

?>