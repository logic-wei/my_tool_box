#!/usr/bin/env python3
import xml.etree.ElementTree as et
import graphviz as gv
import argparse as ap
import os
import shutil
import time


# config
VERSION = "v1.0.2019.9.18"
AUTHOR = "logic-wei"


class Node:
    def __init__(self, name=None, id=None, instance=None, instanceId=None):
        self.name = name
        self.id = id
        self.instance = instance
        self.instanceId = instanceId

    def equals(self, other):
        if self.name == other.name and self.instance == other.instance:
            return True
        else:
            return False

    def graphNodeId(self):
        return r"%s\n%s\n%s\n%s" % (self.name, self.id, self.instance, self.instanceId)
        # return "%s-%s-%s-%s" % \
        #        (self.name, self.id, self.instance, self.instanceId)

    def graphNodeLabel(self):
        return r"%s\n%s\n%s\n%s" % (self.name, self.id, self.instance, self.instanceId)


class Port:
    def __init__(self):
        self.node = Node()
        self.name = None
        self.id = None


class Link:
    def __init__(self):
        self.srcPort = None
        self.dstPorts = []

    def graphEdgeLabel(self, dstIndex):
        return r"src=%s (%s)\ndst=%s (%s)" % \
               (self.srcPort.id, self.srcPort.name,
                self.dstPorts[dstIndex].id, self.dstPorts[dstIndex].name)


class Linkages:
    def __init__(self):
        self.srcNode = Node()
        self.links = []


class Pipeline:
    def __init__(self):
        self.name = None
        self.nodes = []
        self.linkages = None


class Usecase:
    def __init__(self):
        self.name = None
        self.pipelines = []


def parse(xmlfile):
    tree = et.parse(xmlfile)
    root = tree.getroot()
    usecases = []

    if root.tag == "UsecaseDef":
        elem_UsecaseDef = root
        # --------------------UsecaseDef--------------------

        for child in elem_UsecaseDef:
            if child.tag == "Usecase":
                elem_Usecase = child
                # --------------------Usecase--------------------
                cur_usecase = Usecase()
                usecases.append(cur_usecase)

                for child2 in elem_Usecase:
                    if child2.tag == "UsecaseName":
                        elem_UsecaseName = child2
                        # --------------------UsecaseName--------------------
                        cur_usecase.name = elem_UsecaseName.text
                    elif child2.tag == "Pipeline":
                        elem_Pipeline = child2
                        # --------------------Pipeline--------------------
                        cur_pipeline = Pipeline()
                        cur_usecase.pipelines.append(cur_pipeline)

                        for child3 in elem_Pipeline:
                            if child3.tag == "PipelineName":
                                elem_PipelineName = child3
                                # --------------------PipelineName--------------------
                                cur_pipeline.name = elem_PipelineName.text
                            elif child3.tag == "NodesList":
                                elem_NodesList = child3
                                # --------------------NodesList--------------------

                                for child4 in elem_NodesList:
                                    if child4.tag == "Node":
                                        elem_Node = child4
                                        # --------------------Node--------------------
                                        cur_node = Node()
                                        cur_pipeline.nodes.append(cur_node)

                                        for child5 in elem_Node:
                                            if child5.tag == "NodeName":
                                                cur_node.name = child5.text
                                            elif child5.tag == "NodeId":
                                                cur_node.id = child5.text
                                            elif child5.tag == "NodeInstance":
                                                cur_node.instance = child5.text
                                            elif child5.tag == "NodeInstanceId":
                                                cur_node.instanceId = child5.text
                            elif child3.tag == "PortLinkages":
                                elem_PortLinkages = child3
                                # --------------------PortLinkages--------------------
                                cur_linkages = Linkages()
                                cur_pipeline.linkages = cur_linkages

                                for child4 in elem_PortLinkages:
                                    if child4.tag == "SourceNode":
                                        elem_SourceNode = child4
                                        # --------------------SourceNode--------------------
                                        cur_linkages.srcNode.name = elem_SourceNode.text
                                    elif child4.tag == "SourceNodeInstance":
                                        elem_SourceNodeInstance = child4
                                        # --------------------SourceNodeInstance--------------------
                                        cur_linkages.srcNode.instance = elem_SourceNodeInstance.text
                                    elif child4.tag == "Link":
                                        elem_Link = child4
                                        # --------------------Link--------------------
                                        cur_link = Link()
                                        cur_linkages.links.append(cur_link)

                                        for child5 in elem_Link:
                                            if child5.tag == "SrcPort":
                                                elem_SrcPort = child5
                                                # --------------------SrcPort--------------------
                                                cur_srcPort = Port()
                                                cur_link.srcPort = cur_srcPort

                                                for child6 in elem_SrcPort:
                                                    if child6.tag == "PortName":
                                                        cur_srcPort.name = child6.text
                                                    elif child6.tag == "PortId":
                                                        cur_srcPort.id = child6.text
                                                    elif child6.tag == "NodeName":
                                                        cur_srcPort.node.name = child6.text
                                                    elif child6.tag == "NodeId":
                                                        cur_srcPort.node.id = child6.text
                                                    elif child6.tag == "NodeInstance":
                                                        cur_srcPort.node.instance = child6.text
                                                    elif child6.tag == "NodeInstanceId":
                                                        cur_srcPort.node.instanceId = child6.text
                                            elif child5.tag == "DstPort":
                                                elem_DstPort = child5
                                                # --------------------DstPort--------------------
                                                cur_dstPort = Port()
                                                cur_link.dstPorts.append(cur_dstPort)

                                                for child6 in elem_DstPort:
                                                    if child6.tag == "PortName":
                                                        cur_dstPort.name = child6.text
                                                    elif child6.tag == "PortId":
                                                        cur_dstPort.id = child6.text
                                                    elif child6.tag == "NodeName":
                                                        cur_dstPort.node.name = child6.text
                                                    elif child6.tag == "NodeId":
                                                        cur_dstPort.node.id = child6.text
                                                    elif child6.tag == "NodeInstance":
                                                        cur_dstPort.node.instance = child6.text
                                                    elif child6.tag == "NodeInstanceId":
                                                        cur_dstPort.node.instanceId = child6.text
    return usecases


attr_pipeline_graph = {
    "labelloc": "t",
    "fontsize": "30",
    "rankdir": "LR",
    "bgcolor": "azure"
}

attr_srcnode = {
    "shape": "rarrow",
    "fillcolor": "tomato",
    "style": "filled"
}

attr_node = {
    "shape": "box3d",
    "fillcolor": "burlywood1",
    "style": "filled"
}


def renderPipeline(pipeline, path):
    graph = gv.Digraph()
    graph.attr(label=pipeline.name, **attr_pipeline_graph)

    for node in pipeline.nodes:
        if node.equals(pipeline.linkages.srcNode):
            # src node
            graph.node(node.graphNodeId(), label=node.graphNodeLabel(), **attr_srcnode)
        else:
            # normal node
            graph.node(node.graphNodeId(), label=node.graphNodeLabel(), **attr_node)

    for link in pipeline.linkages.links:
        # link
        for index, dstPort in enumerate(link.dstPorts):
            graph.edge(link.srcPort.node.graphNodeId(), dstPort.node.graphNodeId(), link.graphEdgeLabel(index))

    graph.render(path + pipeline.name, cleanup=True)


def main():
    argparser = ap.ArgumentParser()

    argparser.add_argument("file",
                           help="Specify a xml file of usecases to process.")
    argparser.add_argument("path",
                           help="Specify a path to output.")
    argparser.add_argument("-v", "--version",
                           action="version",
                           version=VERSION + " Written by " + AUTHOR + ".")
    argparser.add_argument("-u", "--usecase",
                           help="Specify a usecase name to process.Default all the usecases.")

    args = argparser.parse_args()

    if not os.path.exists(args.path):
        os.mkdir(args.path)
    usecases = parse("./" + args.file)
    os.chdir(args.path)

    time_begin = time.time()
    for usecase in usecases:
        if args.usecase is None or usecase.name == args.usecase:
            print("Rendering for usecase: %s..." % usecase.name)
            if os.path.exists(usecase.name):
                shutil.rmtree(usecase.name)
            os.mkdir(usecase.name)
            for index, pipeline in enumerate(usecase.pipelines):
                prefix = "├── "
                if index == len(usecase.pipelines)-1:
                    prefix = "└── "
                print(prefix + "Pipeline: " + pipeline.name)
                renderPipeline(pipeline, "./" + usecase.name + "/")
            print("")
    print("Done. (%s seconds)" % str(str(time.time() - time_begin)))


if __name__ == "__main__":
    main()
