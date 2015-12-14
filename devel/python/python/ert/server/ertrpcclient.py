import socket
from xmlrpclib import ServerProxy, Fault

FAULT_CODES = {1: UserWarning,
               2: KeyError,
               3: IndexError,
               4: LookupError}


def convertFault(fault):
    if fault.faultCode in FAULT_CODES:
        fault_type = FAULT_CODES[fault.faultCode]
        raise fault_type(fault.faultString)
    else:
        raise fault


class ErtRPCClient(object):
    def __init__(self, host, port, verbose=False):
        self._server_proxy = ServerProxy("http://%s:%s" % (host, port), allow_none=True, verbose=verbose)
        socket.setdefaulttimeout(180) # 3 minutes

    def ertVersion(self):
        """
         Returns a version tuple: (major, minor, micro)
         @rtype: tuple
        """
        return tuple(self._server_proxy.ertVersion())

    def getTimeMap(self, target_case_name):
        """
         Returns a list of datetime objects for the named target case name
         @type target_case_name: str
         @rtype: list of datetime
        """
        return self._server_proxy.getTimeMap(target_case_name)


    def isRunning(self):
        """
        Returns True if a simulation batch has been started and is running.
        @rtype: bool
        """
        return self._server_proxy.isRunning()


    def isInitializationCaseAvailable(self):
        """
        Returns True if the initialization case is prepared and ready to run simulations.
        @rtype: bool
        """
        return self._server_proxy.isInitializationCaseAvailable()


    def startSimulationBatch(self, initialization_case_name, simulation_count):
        """
        Start a simulation batch. Will prepare a batch that will run for the specified number of realizations.
        Will fail if the server is already running a batch or no initialization case is available.
        @param initialization_case_name: The case containing geo realizations
        @type initialization_case_name: str
        @type simulation_count: int
        """
        try:
            self._server_proxy.startSimulationBatch(initialization_case_name, simulation_count)
        except Fault as f:
            raise convertFault(f)


    def addSimulation(self, target_case_name, geo_id, pert_id, sim_id, keywords):
        """
        Start a simulation.
        @type target_case_name: str
        @type geo_id: int
        @type pert_id:
        @type sim_id: int
        @type keywords: dict[str, list]
        @raise UserWarning if the server is not ready to receive simulations
        @raise UserWarning if the server is already running a simulation with the same id as sim_id
        """
        try:
            self._server_proxy.addSimulation(target_case_name, geo_id, pert_id, sim_id, keywords)
        except Fault as f:
            raise convertFault(f)


    def isRealizationFinished(self, sim_id):
        """
        Returns true if the realization is finished running.
        @type sim_id: int
        @rtype: bool
        """
        return self._server_proxy.isRealizationFinished(sim_id)

    def didRealizationSucceed(self, sim_id):
        """
        Check if the realization successfully finished running.
        @type sim_id: int
        @rtype: bool
        """
        return self._server_proxy.didRealizationSucceed(sim_id)

    def didRealizationFail(self, sim_id):
        """
        Check if the realization failed while running.
        @type sim_id: int
        @rtype: bool
        """
        return self._server_proxy.didRealizationFail(sim_id)

    def getGenDataResult(self, target_case_name, sim_id, report_step, keyword):
        """
        Retrieve a GenData result from a target case
        @type target_case_name: str
        @type sim_id: int
        @type report_step: int
        @type keyword: str
        @rtype: list[float]
        @raise KeyError if the server was unable to recognize the keyword
        @raise UserWarning if the server was unable to load the data
        @raise UserWarning if the simulation (with sim_id) is still running
        @raise UserWarning if the keyword is not of the correct type
        """
        try:
            return self._server_proxy.getGenDataResult(target_case_name, sim_id, report_step, keyword)
        except Fault as f:
            raise convertFault(f)

    def getCustomKWResult(self, target_case_name, sim_id, keyword):
        """
        Retrieve a CustomKW result from the target case.
        @type target_case_name: str
        @type sim_id: int
        @type keyword: str
        @rtype: dict[str, Union[float,str]]
        @raise KeyError if the server was unable to recognize the keyword
        @raise UserWarning if the server was unable to load the data
        @raise UserWarning if the simulation (with sim_id) is still running
        @raise UserWarning if the keyword is not of the correct type
        """
        try:
            return self._server_proxy.getCustomKWResult(target_case_name, sim_id, keyword)
        except Fault as f:
            raise convertFault(f)

    def isCustomKWKey(self, key):
        """
        Check if a key is of CustomKW type
        @param key: The key to check
        @type key: str
        @rtype: bool
        """
        return self._server_proxy.isCustomKWKey(key)

    def isGenDataKey(self, key):
        """
        Check if a key is of CustomKW type
        @param key: The key to check
        @type key: str
        @rtype: bool
        """
        return self._server_proxy.isGenDataKey(key)
